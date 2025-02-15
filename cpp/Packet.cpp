//
#include "Packet.h"
#include "Common.h"
#include "serdes.h"
#include "Messages.h"
#include "Logger.h"
#include "MsgExt.h"

#include <iostream>
#include <string>
#include <sstream>

Packet::Packet() :
	pdata(nullptr),
	allocatedLength(0),
	usedLength(0)
{
}

Packet::Packet(const int size) :
	pdata(nullptr),
	allocatedLength(0),
	usedLength(0)
{
	init(size);
}

void Packet::init(const int size)
{
	usedLength = 0;

	// if no change in size, use previous allocation
	if ((pdata) && (allocatedLength == size)) return;

	// else allocate
	if (pdata) {
		delete[] pdata;	// if already allocate
		pdata = 0;
	}
	allocatedLength = size ;
	pdata = size>0 ? new char[size] : 0;
}

void Packet::deinit() {
	if (pdata != nullptr) {
		delete[] pdata;
		pdata = nullptr;
	}
	allocatedLength = 0;
	usedLength = 0;
}

Packet::~Packet() {
	deinit();
}

// copy constructor
Packet::Packet(const Packet& other) :
	pdata(new char[other.allocatedLength]),
	allocatedLength(other.allocatedLength),
	usedLength(other.usedLength)
{
	std::copy(other.pdata, other.pdata + allocatedLength, pdata);
}

// copy assignment operator
Packet& Packet::operator=(const Packet& other) {
	if (this != &other) {
		if (pdata != nullptr) delete[] pdata;
		allocatedLength = other.allocatedLength;
		pdata = new char[other.allocatedLength];
		std::copy(other.pdata, other.pdata + other.allocatedLength, pdata);
		usedLength = other.usedLength;
	}
	return *this;
}

// move constructur
Packet::Packet(Packet&& other) :
	allocatedLength(other.allocatedLength),
	usedLength(other.usedLength)
{
	pdata = other.pdata;
	other.pdata = nullptr;
	other.usedLength = 0;
	other.allocatedLength = 0;
}

// move assignment operator
Packet& Packet::operator=(Packet&& other) {
	if (this != &other) {
		if (pdata != nullptr) delete[] pdata;
		allocatedLength = other.allocatedLength;
		usedLength = other.usedLength;
		pdata = other.pdata;
		other.pdata = nullptr;
		other.usedLength = 0;
		other.allocatedLength = 0;
	}
	return *this;
}

Packet::operator std::string() {
	std::string _string = "usedLength:" + std::to_string(usedLength);
	return _string;
}

// ------------------------------------------

// --------------------------------------------------------------------
RecvPacketBuilder::RecvPacketBuilder(IReadDataObject& _ireadDataObject, int _defaultPacketLength) :
	ireadDataObject{ _ireadDataObject }, defaultPacketLength{ _defaultPacketLength}
{
	// compute Msg length
	packet.init(COMMON_PACKETSIZE_MAX);
	poffset = porigin = packet.getpData();
	SerDes sd(porigin, packet.getAllocatedLength());
	Msg msg;
	MSGLENGTH = msg.serialize(sd);

	reset();
}

void RecvPacketBuilder::init(void) {
	reset();
}

void RecvPacketBuilder::deinit(void) {
	packet.deinit();
	poffset = porigin = 0;
	neededLength = 0;
}

void RecvPacketBuilder::reset(void) {
	packet.init(defaultPacketLength);
	poffset = porigin = packet.getpData();
	neededLength = MSGLENGTH;
}

bool RecvPacketBuilder::isHeaderRead(void) {
	if ((poffset - porigin) < (long int)MSGLENGTH) return false;
	return true;
}

bool RecvPacketBuilder::isNumReadEnough(int numRead) {
	neededLength -= numRead;
	poffset += numRead;
	if (neededLength > 0) { // data too short
		return false;
	}
	return true;
}

Packet& RecvPacketBuilder::getPacket(void) {
	packet.setUsedLength(getUsedDataLength());
	return packet;
}

RecvPacketBuilder::READSTATUS  RecvPacketBuilder::recvNeededData(void) {
	int resLen = ireadDataObject.readData(poffset, (uint32_t)neededLength);
	if (resLen < 0) return READSTATUS::READERROR;
	if (resLen == 0) return READSTATUS::PEERSHUTDOWN;
	if (!isNumReadEnough((int)resLen))
		return READSTATUS::MOREDATANEEDED;
//	assert(neededLength == 0);
	return READSTATUS::NEEDEDDATAREAD;
}

[[maybe_unused]] static Msg debugLastMsg;
[[maybe_unused]] static int debugLastNeededLength = 0;

RecvPacketBuilder::READSTATUS  RecvPacketBuilder::readPacket(Packet& packetRead)
{
	if (!isHeaderRead()) {
		// repeat read until full msg header is received
		READSTATUS readStatus = recvNeededData();
		if (readStatus!=READSTATUS::NEEDEDDATAREAD)
			return readStatus;
		LOG_ASSERT(poffset == (porigin + MSGLENGTH));

		// get packet length
		SerDes sdMsg(porigin, MSGLENGTH);
		Msg msg;
		msg.deserialize(sdMsg);

		// TOTO remove debug Code
		//Msg* pdebugLastMsg = &debugLastMsg;
		//int* pdebugLastNeededLength = &debugLastNeededLength;
		if (msg.prot!=PROTOCOL_CODE) {
			std::unique_ptr<MsgDiagnostics> pMsgDiagnostic =
				std::make_unique<MsgDiagnostics>(debugLastMsg, msg, porigin, "RecvPacketBuilder::readPacket(): packet invalid protocol code");
			throw pMsgDiagnostic;
		}
		if (msg.crc != msg.calcCrc()) {
			std::unique_ptr<MsgDiagnostics> pMsgDiagnostic = 
				std::make_unique<MsgDiagnostics>(debugLastMsg, msg, porigin, "RecvPacketBuilder::readPacket(): packet invalid crc");
			throw pMsgDiagnostic;
		}
		neededLength = msg.length - MSGLENGTH;
		// TOTO remove debug Code
		debugLastMsg = msg;
		debugLastNeededLength = neededLength;
	}
	if (isHeaderRead()) {
		// repeat read until full msg body is received
		READSTATUS readStatus = recvNeededData();
		if (readStatus != READSTATUS::NEEDEDDATAREAD)
			return readStatus;
		packetRead = getPacket();
		reset();
	}
	return READSTATUS::NEEDEDDATAREAD;
}

// -----------------

WritePacketBuilder::WritePacketBuilder(int _defaultPacketLength) :
	defaultPacketLength{ _defaultPacketLength }
{
	init();
}

void WritePacketBuilder::init(void)
{
	packet.init(defaultPacketLength);
	offset = 0;
	porigin = packet.getpData();
}

void WritePacketBuilder::deinit(void)
{
	packet.deinit();
	offset = 0;
	porigin = 0;
	defaultPacketLength = 0;
}

bool WritePacketBuilder::addNew(Packet& _packet)
{
	packet = _packet;
	porigin = packet.getpData();
	offset = 0;

	// TODO remove this debug code
	//dumpToBotBaseForDbg();

	return true;
}

/*
void WritePacketBuilder::dumpToBotBaseForDbg(void)
{
	#ifdef _WIN64
	SerDes sdMsg(porigin, sizeof(Msg));
	Msg msg;
	msg.deserialize(sdMsg);
	LOG_DBGD("wrote", MsgExt(msg), 0);
	#endif
}
*/

char* WritePacketBuilder::getpData(void) const {
	return (porigin + offset);
}

int WritePacketBuilder::getLength(void) const {
	int len = packet.getUsedLength() - offset;
	return len;
}

bool WritePacketBuilder::setNumSent(int numSent) {
	offset += numSent;
	return empty();
}

bool WritePacketBuilder::empty(void) const {
	return (getLength() == 0);
}
