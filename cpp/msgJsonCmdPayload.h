#pragma once

#include "Packet.h"
#include "MsgExt.h"

enum COMMANDS {
	CMD_NONE,
	PROC_SUSPEND,
	ADMIN_UPLOADSTATE,
	ADMIN_UPLOADSTATS,
	ADMIN_UPLOADLOGS,
	CMD_TOGGLEMATRXIGUI,
};

class Cmd {
	//static map <std::string, COMMANDS> g_mapStringToCommands;
	std::map <std::string, COMMANDS > g_mapStringToCommands;
	std::string codeString;
public:
	COMMANDS command;
	Cmd(std::string jsonString);
	~Cmd();

	void fromJsonString(std::string jsonString);
	std::string toJsonString(void);
};

class MsgJsonCmdPayload
{
	template <typename T>
	static bool getCommandFromJson(T& obj, const json& jsonData, const std::string& command);
	template <typename T, typename U>
	static bool getObjFromSrcObjJson(T& obj, U& srcObj, const std::string& command);
public:
//    bool onNewPacket(PacketEx& packetEx);
//    virtual bool decode(PacketEx& packetEx);
	static bool decode(MsgJsonCmd& rmsgJsonCmd, std::unique_ptr<CommonInfoBase> &pcommonInfoBase, HYPERCUBECOMMANDS &command);
	static bool decode(MsgContext& msgContext);
};



