//
#include "MsgExt.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>
#if defined(WIN32) || defined(WIN64)
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
//#include "bits/stdc++.h"

#include "Logger.h"
#include "Common.h"
#include "Packet.h"

MsgExt::MsgExt(Msg ext) :
	Msg{ext}
{
}


MsgExt::~MsgExt()
{
}

MsgExt::operator std::string() {
	std::stringstream ss;
	ss << "P: " << prot;
	ss << ", F: " << flags;
	ss << ", L: " << length;
	ss << ", D: " << std::hex << deviceAppKey;
	ss << ", SE: " << sessionKey;
	ss << ", SQ: " << seqNumber;
	switch (subSys) {
	case SUBSYS_DISCOVERY:
		switch (command) {
		case DISCOVERY_HELLO:
			ss << ", S: DIS, C: HELLO";
			break;
		case DISCOVERY_HELLOACK:
			ss << ", S: DIS, C: HELLOACK";
				break;
		case DISCOVERY_CLOSESOCKET:
			ss << ", S: DIS, C: CLOSESOCKET";
				break;
		default:
			ss << ", S: DIS, C: " << command;
				break;

		}
	case SUBSYS_STATS:
		switch (command) {
		case STATS_STATINFO:
			ss << ", S: STATS, C: STATINFO";
			break;
		case STATS_IDDITEMSET:
			ss << ", S: STATS, C: IDDITEMSET";
			break;
		default:
			ss << ", S: STATS, C: " << command;
			break;

		}
	case SUBSYS_CMD:
		switch (command) {
		case  CMD_PCJSON:
			ss << ", S: CMD, C: PCJSON";
			break;
		case  CMD_PINGFROMPC:
			ss << ", S: CMD, C: PINGFROMPC";
			break;
		case  CMD_PINGFROMPCACK:
			ss << ", S: CMD, C: PINGFROMPCACK";
			break;
		case  CMD_PINGTOPC:
			ss << ", S: CMD, C: PINGTOPC";
			break;
		case  CMD_PINGTOPCACK:
			ss << ", S: CMD, C: PINGTOPCACK";
			break;
		default:
			ss << ", S: CMD, C: " << command;
			break;

		}
	case SUBSYS_OBJ:
		switch (command) {
		case  OBJ_APPMGR_APPINFOLIST:
			ss << ", S: OBJ, C: APPINFO";
			break;
		default:
			ss << ", S: CMD, C: " << command;
			break;
		}
	default:
		ss << ", S: " << subSys << ", C: " << command;
		break;
	}

	ss << ", A: " << argument;
	ss << ", c: " << std::hex << crc;
	return ss.str();
}

std::unique_ptr<Msg> MsgExt::factoryMethod(const PacketEx& rpacket)
{
	char* pdata = rpacket.packet.getpData();
	int lengthOffset = sizeof(prot) + sizeof(flags);
	int subSysOffset = lengthOffset + sizeof(length);
	int commandOffset = subSysOffset + sizeof(subSys);
	int length = *((int*)(pdata + lengthOffset));
	short int subSys = *((short int*)(pdata + subSysOffset));
	short int command = *((short int*)(pdata + commandOffset));
	length = ntohl(length);
	subSys = ntohs(subSys);
	command = ntohs(command);

	SerDes sd(pdata, rpacket.packet.getUsedLength());
	std::unique_ptr<Msg> pmsg;

	switch (subSys) {
	case SUBSYS_DISCOVERY:
		break;
	case SUBSYS_STATS:
		break;
	case SUBSYS_CMD:
		{
			pmsg = std::make_unique<MsgCmd>("");
			pmsg->deserialize(sd);
			short int _calcCrc = pmsg->calcCrc();
			LOG_ASSERT(pmsg->crc == _calcCrc);
			switch (command) {
				case CMD_PCJSON:
					break;
				default:
					break;
				}
		}
		break;
	case SUBSYS_OBJ:
		{
			std::unique_ptr<MsgObjectEx> pmsgOx = std::make_unique<MsgObjectEx>((DEVICEID)rpacket.deviceId, SUBSYS_OBJ, 0, "");
			pmsgOx->deserialize(sd);
			short int _calcCrc = pmsgOx->calcCrc();
			LOG_ASSERT(pmsgOx->crc == _calcCrc);

			switch (command) {
				case OBJ_LOGGER:
				case OBJ_MATRIXMGR:
				default:
					pmsgOx->decode();
					break;
			}
			pmsg = std::move(pmsgOx);
		}
		break;
	default:
		break;
	}
	return pmsg;
}

// -------------------------------------

MsgCmdExt::MsgCmdExt(std::string command, json data) :
	MsgCmd("")
{
	json cmd = { { "command", command }, { "data", data } };
	jsonData = cmd.dump();
}

// -------------------------------------------

MsgObjectEx::MsgObjectEx(DEVICEID _deviceId, int objDom, int objId, String objString) :
	MsgObject(objDom, objId, objString), deviceId{ _deviceId } {
}

MsgObjectEx::MsgObjectEx(DEVICEID _deviceId, int objDom, int objId, int _code, int _refNum, json _jdata, bool _status, std::string _message) :
	MsgObject(objDom, objId, ""), deviceId{ _deviceId }  {
	code = _code;
	refNum = _refNum;
	jdata = _jdata;
	status = _status;
	message = _message;
	encode();
}

MsgObjectEx::MsgObjectEx::MsgObjectEx(const MsgObjectEx& other) :
	MsgObject(other.command, other.objectId, other.jsonObjectString),
	deviceId{ other.deviceId},
	code { other.code },
	refNum{ other.refNum},
	jdata{ other.jdata },
	status{ other.status},
	message{ other.message}
{
	prot = other.prot;
	length = other.length;
	deviceAppKey = other.deviceAppKey;
	sessionKey = other.sessionKey;
	seqNumber = other.seqNumber;
	subSys = other.subSys;
	command = other.command;
	argument = other.argument;
	crc = other.crc;
	decode();
}

void MsgObjectEx::decode(void) {
	std::string jsonString = jsonObjectString;
	json jmsgObjectEx = json::parse(jsonString.c_str());

	try {
		code = jmsgObjectEx["code"];
		refNum = jmsgObjectEx["refNum"];
	}
	catch (std::exception const& e) {
		LOG_ERROR("MsgObjectEx::decode()", "missing field" + std::string(e.what()), 0);
	}
	try {
		jdata = jmsgObjectEx["data"];
		status = jmsgObjectEx["status"];
		message = jmsgObjectEx["message"];
	}
	catch (...) {
	}
}

void MsgObjectEx::encode(void) {
	json jmsgObjectEx;

	jmsgObjectEx["code"] = code;
	jmsgObjectEx["refNum"] = refNum;
	jmsgObjectEx["data"] = jdata;
	jmsgObjectEx["status"] = status;
	jmsgObjectEx["message"] = message;

	jsonObjectString = jmsgObjectEx.dump();
}


// ------------------------------------------------
bool MsgStaticTests::testMsgSize(Msg* pmsg)
{
	static Packet pout(COMMON_PACKETSIZE_MAX);
	SerDes sdo(pout.getpData(), pout.getLength());
	int used = pmsg->serialize(sdo);
	if (used != pmsg->size()) return false;
	return true;
}

bool MsgStaticTests::runTests(void)
{

	// test message sizes are correct
	Msg msg;
	//int msgSize = msg.size();

	if (!testMsgSize(&msg)) return false;

	MsgPCInfo msgPCInfo;
	msgPCInfo.name = "test";
	msgPCInfo.ipAddress = "192.168.1.12";
	msgPCInfo.ipGateway = "192.168.1.1";
	msgPCInfo.port = 21;
	if (!testMsgSize(&msgPCInfo)) return false;

	MsgDiscoveryMulticastHello _MsgDiscoveryMulticastHello;
	if (!testMsgSize(&_MsgDiscoveryMulticastHello)) return false;

	MsgDiscoveryHelloAck msgDiscoveryHelloAck;
	if (!testMsgSize(&msgDiscoveryHelloAck)) return false;

	MsgIddStatItem msgIddStatItem;
	msgIddStatItem.statId = 3;
	msgIddStatItem.time = 1;

	MsgIddStatItemSet msgIddStatItemSet;
	msgIddStatItemSet.name = "test2";
	msgIddStatItemSet.description = "stat item set";
	msgIddStatItemSet.statList.push_back(msgIddStatItem);
	msgIddStatItemSet.statList.push_back(msgIddStatItem);
	msgIddStatItemSet.statList.push_back(msgIddStatItem);
	if (!testMsgSize(&msgIddStatItemSet)) return false;

	MsgStatInfo msgStatInfo;
	msgStatInfo.jsonStatInfoString = "large json string is in here";
	if (!testMsgSize(&msgStatInfo)) return false;

	MsgCmd msgCmd("test");
	if (!testMsgSize(&msgCmd)) return false;

	MsgObject msgObject(OBJ_LOGGER, OBJ_LOGGER_LOGLINE, "log line");
	msgObject.jsonObjectString = "large json string is in here";
	if (!testMsgSize(&msgObject)) return false;

	return true;
}

// ------

MsgDiagnostics::MsgDiagnostics(const Msg& _lastMsg, const Msg& _currentMsg, const char* _pcurrentPacketRead, std::string _errorMsg)
	: lastMsg(_lastMsg), currentMsg(_currentMsg), pcurrentPacketRead(_pcurrentPacketRead), errorMsg(_errorMsg)
{

}

std::string MsgDiagnostics::dumpReport(void) {
	std::stringstream ss;
	ss << "EXCEPTION" << errorMsg << "\n";
	std::string lastMsgString = lastMsg;
	std::string currentMsgString = currentMsg;
	ss << "LastMsg\n" << lastMsgString << "\n";
	ss << "currentMsg\n" << currentMsgString << "\n";
	const char* pdata = pcurrentPacketRead;
	ss << std::hex << "HexDump\n";
	for (int i=0; i<32; i++) {
		ss << (unsigned char)*pdata++ << " ";
	}
	ss << "\n";
	pdata = pcurrentPacketRead;
	short int prot = *(short int*)pdata; pdata += 2;
	short int flags = *(short int*)pdata; pdata += 2;
	short int length = *(int*)pdata++; pdata += 4;
	ss << "DECODED\n";
	ss << "P " << prot << " F " << flags << " L " << length;
	return ss.str();
}
