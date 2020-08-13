
/* -------------------------------------------------------------------------------------------

   This is common code used by both the C++ compiler and then coverted to Dart code
   The code used here is carefully chosen to be easily coverted to Dart code using 
   sed and a sed script. 

   To use in C++
   This file is included in the Message.h file and by default is standard C++.

   To use in Dart
   Run the Generate.bat file that will covert this file to an equivalent Dart file.
   Essentially the sed script does the following:

    (*) add a line to import the needed SerDes Serialize Deserialize class
    (*) replace }; to remove ; from class declaration endings
    (*) replace ": public" Keyword with "extends" - for Dart base classes
    (*) remove "public:" Keyword - Dart methods are public depending on namee
    (*) replace "std::string" Keyword with "String" - for Dart String objects
    (*) replace "short int" with "int" - Dart has no short int
    (*) replace "SerDes&" with "SerDes" - Dart all objects passed by reference
    (*) replace Msg:: with super. - This is NOT generic and ONLY works for Msg
            IF Other classes are needed - then the script will need to be modified.

    Ravi Rajapakse

    -------------------------------------------------------------------------------------------
*/

const int PROTOCOL_CODE = 1122;

const int SUBSYS_DISCOVERY      = 1;
const int DISCOVERY_HELLO       = 1;
const int DISCOVERY_HELLOACK    = 2;   // not used
const int DISCOVERY_CLOSESOCKET = 3;   // notify other end that connection is to be ended

const int SUBSYS_STATS          = 2;
const int STATS_STATINFO        = 1;
const int STATS_IDDITEMSET      = 2;

const int SUBSYS_CMD            = 3;
const int CMD_PCJSON            = 1;

const int SUBSYS_OBJ                = 4;
const int OBJ_DOM_UNKNOWN           = 0;
const int OBJ_ID_UNKNOWN            = 0;
const int OBJ_DOM_APPMGR            = 1;
const int OBJ_ID_APPMGRINFO         = 1;

// Base message class. This can be moved out of here if it needs to be specific to each
// Language. For the moment it seems it can be common.

class Msg {
public:
    short int prot;     // this should alway be set to the PROTOCOL_CODE
    short int length;   // this will be updated by the SerDes class on calls to length()
    short int subSys;   // to be set by derived classes
    short int command;  // to be set by derived classes
    int seqNumber;
    Msg() {
        prot = PROTOCOL_CODE;
        length = 0;
        subSys = 0;
        command = 0;
        seqNumber = 0;
    }
    virtual int serialize(RSerDes sd) {
        sd.setInt16(prot);
        sd.setLength16(length);
        sd.setInt16(subSys);
        sd.setInt16(command);
        sd.setInt32(seqNumber);
        return sd.length();
    }
    virtual int deserialize(RSerDes sd) {
        prot = sd.getProtocolCodeAndCheckEndian(PROTOCOL_CODE);
        length = sd.getInt16();
        subSys = sd.getInt16();
        command = sd.getInt16();
        seqNumber = sd.getInt32();
        return sd.length();
    }
};

class MsgPCInfo :public Msg {
public:
    int64_t uid;
    String name;
    String ipAddress;
    String ipGateway;
    int port;
    MsgPCInfo() {
        uid = 0;
        name = "";
        ipAddress = "";
        ipGateway = "";
        port = 0;
    }
    virtual int serialize(RSerDes sd) {
        M_BASECLASS(Msg,serialize(sd));
        sd.setInt64(uid);
        sd.setString(name);
        sd.setString(ipAddress);
        sd.setString(ipGateway);
        sd.setInt32(port);
        return sd.length();
    }
    virtual int deserialize(RSerDes sd) {
        M_BASECLASS(Msg,deserialize(sd));
        uid = sd.getInt64();
        name = sd.getString();
        ipAddress = sd.getString();
        ipGateway = sd.getString();
        port = sd.getInt32();
        return sd.length();
    }
};

class MsgDiscoveryMulticastHello : public MsgPCInfo {
public:
    MsgDiscoveryMulticastHello() {
        subSys = SUBSYS_DISCOVERY;
        command = DISCOVERY_HELLO;
    }
};

class MsgDiscoveryHelloAck : public MsgPCInfo {
public:
    MsgDiscoveryHelloAck() {
        subSys = SUBSYS_DISCOVERY;
        command = DISCOVERY_HELLOACK;
    }
};

// ------------------------------------------------------------------

class MsgIddStatItem {
public:
    short int statId;
    short int groupId;
    short int streamId;
    int value;
    int timeStamp;
    MsgIddStatItem() {
        value = 0;
        timeStamp = 0;
        statId = 0;
        groupId = 0;
        streamId = 0;
    }
    void serialize(RSerDes sd) {
        sd.setInt16(statId);
        sd.setInt16(groupId);
        sd.setInt16(streamId);
        sd.setInt32(value);
        sd.setInt32(timeStamp);
    }
    void deserialize(RSerDes sd) {
        statId = sd.getInt16();
        groupId = sd.getInt16();
        streamId = sd.getInt16();
        value = sd.getInt32();
        timeStamp = sd.getInt32();
    }
};

class MsgIddStatItemSet :public Msg {
public:
    String name;
    String description;
    int id;
    int listLength;
    List<MsgIddStatItem> statList;
    MsgIddStatItemSet() {
        subSys = SUBSYS_STATS;
        command = STATS_IDDITEMSET;
        name = "";
        description = "";
        id = 0;
        listLength = 0;
        M_ALLOCATELIST(MsgIddStatItem,statList)
	}
    int serialize(RSerDes sd) {
        listLength = M_LISTLEN(statList);
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(name);
        sd.setString(description);
        sd.setInt32(id);
        sd.setInt32(listLength);
        M_LISTFORLOOPSTART(statItem,statList)
            statItem.serialize(sd);
		}
        return sd.length();
    }
    
    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        name = sd.getString();
        description = sd.getString();
        id = sd.getInt32();
        listLength = sd.getInt32();
        for (int i = 0; i < listLength; i++) {
            M_DECLAREVARIABLE(MsgIddStatItem,statItem);
            statItem.deserialize(sd);
            statList.add(statItem);
        }
        return sd.length();
    }
};

class MsgStatInfo :public Msg {
public:
    String jsonStatInfoString;
    MsgStatInfo() {
        subSys = SUBSYS_STATS;
        command = STATS_STATINFO;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(jsonStatInfoString);
        return sd.length();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        jsonStatInfoString = sd.getString();
        return sd.length();
    }
};


//---------------------------------------

class MsgCmd :public Msg {
public:
    String jsonCmdString;
    MsgCmd(String cmdString) {
        subSys = SUBSYS_CMD;
        command = CMD_PCJSON;
        jsonCmdString = cmdString;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(jsonCmdString);
        return sd.length();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        jsonCmdString = sd.getString();
        return sd.length();
    }
};


class MsgObject :public Msg {
public:
    String jsonObjectString;
    int objectId;
    MsgObject(int objDom, int objId, String objString) {
        subSys = SUBSYS_OBJ;
        command = objDom;
        objectId = objId;
        jsonObjectString = objString;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setInt32(objectId);
        sd.setString(jsonObjectString);
        return sd.length();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        objectId = sd.getInt32();
        jsonObjectString = sd.getString();
        return sd.length();
    }
};
