#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

#include "Packet.h"
#include "Messages.h"
#include "TContext.h"
#include <string>

#include "json/json.hpp"
using json = nlohmann::json;

class MsgContext;

// This is an extended version of the Msg object used by both C++ and Dart
// This extended object is only used by the Matrix and has parameters relavant to its use
class MsgExt : public Msg {
public:
	MsgExt(Msg ext);
	~MsgExt();
	operator std::string();
	static std::unique_ptr<Msg> factoryMethod(const PacketEx& rpacket, short int& subSys, short int& command);
	static std::unique_ptr<Msg> decodePacketInContext(MsgContext& msgContext);
	static bool checkMsgJsonCmd(MsgJsonCmd& rmsgJsonCmd);
};

class MsgCmdExt : public MsgCmd {
public:
	MsgCmdExt(std::string command, json data);
};

class MsgObjectEx :public MsgObject {
public:
	MsgObjectEx(const MsgObjectEx& other);
	MsgObjectEx(DEVICEID _deviceId, int objDom, int objId, String objString);
	MsgObjectEx(DEVICEID _deviceId, int objDom, int objId, int _code, int _refNum, json _jdata, bool _status = true, std::string _message = "");
	void decode(void);
	void encode(void);
	int size() { return MsgObject::size() + sizeof(deviceId); }
	DEVICEID deviceId = DEVICEID::ALLDEVICES;
	int code = 0;
	int refNum = 0;
	json jdata;
	bool status = true;
	std::string message = "";
};

class MsgStaticTests {
	bool testMsgSize(Msg* pmsg);
public:
	bool runTests(void);
};

class MsgDiagnostics {
	public:
	MsgExt lastMsg;
	MsgExt currentMsg;
	const char* pcurrentPacketRead;
	std::string errorMsg;

	MsgDiagnostics(const Msg& _lastMsg, const Msg& _currentMsg, const char* _pcurrentPacketRead, std::string _errorMsg);

	// MsgDiagnostics();
	std::string dumpReport(void);
};

/** -----------------------------------------------------------------
 * @brief hold all context info for an incomming packet as its decoded.
 * This is done to speed up decoding by providing all the information
 * thats needed in one place and avoid recomputing it.
*/
class MsgContext {
	public:
		// -----------------------------------------------------------
		// STAGE 0 : originating / input data
		TPacketSharedPtr ppacket;			// original packet data

		// STAGE 1: data from message decoding input data
		std::unique_ptr<Msg> pmsg = nullptr;// new decoded message
		short int subSys = 0;				// copy of decoded info
		short int command = 0;				// copy of decoded info
		bool decodedPayload = false;		// decoded payload info

		// STAGE 2: decoded from msg payload
		HyperCubeCommand hyperCubeCommand;
		std::unique_ptr<CommonInfoBase> pcommonInfoBase;		// decoded payload info
		// -----------------------------------------------------------

		MsgContext();
		MsgContext(TPacketSharedPtr _ppacket);
		~MsgContext();

		bool checkMsgJsonCmd(MsgJsonCmd&);
		bool decodePacketToMsg(void);
		bool decodePacketToMsg(PacketEx&);
		bool decodeMsgToHyperCubeCommand(void);
};