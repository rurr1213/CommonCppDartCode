#include <stdlib.h>
#include <assert.h>
#include <string>

#include "Common.h"
#include "Messages.h"
#include "mserdes.h"
#include "MsgExt.h"

#include "json.hpp"

#include <iostream> 
#include <string> 
#include <sstream> 

using json = nlohmann::json;

MSerDes::MSerDes() :
    SerDes( 0, 0)
{

}

std::string MSerDes::to_string(const Msg& rmsg)
{
    std::string line;
    line = "L:" + std::to_string(rmsg.length);
    line = " S:" + std::to_string(rmsg.subSys);
    line = " C:" + std::to_string(rmsg.subSys);
    return line;
}

bool MSerDes::msgToPacket(Msg& msg, Packet* ppacket)
{
    ppacket->init(COMMON_PACKETSIZE_MAX);
    SerDes::init(ppacket->getpData(), ppacket->getLength());
    lastUsedLength = msg.serialize(*this);
    assert(COMMON_PACKETSIZE_MAX > lastUsedLength);
    ppacket->setUsedLength(lastUsedLength);
    return true;
}

bool MSerDes::packetToMsg(const Packet* ppacket, Msg& rmsg)
{
    SerDes::init(ppacket->getpData(), ppacket->getLength());
    rmsg.deserialize(*this);
    return true;
}


bool MSerDes::packetToMsgJson(const Packet* ppacket, MsgJson& rmsgJson, json& _json)
{
    SerDes::init(ppacket->getpData(), ppacket->getLength());
    rmsgJson.deserialize(*this);
    short int calcCrc = rmsgJson.calcCrc();
    if (rmsgJson.crc != calcCrc) {
        std::stringstream ss;
        ss << "LastMsg S:" << rmsgJson.subSys << " C:" << rmsgJson.command << " A:" << rmsgJson.argument << " L:" << rmsgJson.length << "\n";
        ss << rmsgJson.jsonData;
        std::string errorMsg = "json CRC failed " + ss.str();
        throw std::runtime_error(errorMsg);
    }
    try {
       if (rmsgJson.jsonData.find("{")!=std::string::npos) {
            _json = json::parse(rmsgJson.jsonData);    
            return true;
        } else {
            return false;
        }
    } catch(std::exception& e) {
        std::cout << "ERROR! - MSerDes::packetToMsgJson, Failed to decode json " << std::string(e.what()) << "\n";
        assert(false);
        return false;
    }        
    return true;
}
