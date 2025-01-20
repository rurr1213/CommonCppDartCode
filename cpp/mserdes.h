#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string>
#include <memory>

#include "serdes.h"
#include "Packet.h"
#include "Common.h"
#include "Messages.h"

#include "json.hpp"

using json = nlohmann::json;

class Msg;
class Packet;

class MSerDes : public SerDes {
    int lastUsedLength = 0;
    public:
       MSerDes();
       bool msgToPacket(Msg& rmsg, const Packet::UniquePtr& ppacket) { return msgToPacket(rmsg, ppacket.get()); };
       bool msgToPacket(Msg& rmsg, const Packet::SharedPtr& ppacket) { return msgToPacket(rmsg, ppacket.get()); };
       bool msgToPacket(Msg& rmsg, Packet* ppacket);
       bool packetToMsg(const Packet* ppacket, Msg& rmsg);
       bool packetToMsgJson(const Packet* ppacket, MsgJson& rmsgJson, json& _json);
       int getLastPacketSize(void) { return lastUsedLength; }
       std::string to_string(const Msg& rmsg);
};

