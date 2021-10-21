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

inline const int PROTOCOL_CODE = 1122;

inline const int SUBSYS_DISCOVERY          = 1;
inline const int DISCOVERY_HELLO           = 1;
inline const int DISCOVERY_HELLOACK        = 2;   // not used
inline const int DISCOVERY_CLOSESOCKET     = 3;   // notify other end that connection is to be ended

inline const int SUBSYS_STATS              = 2;
inline const int STATS_STATINFO            = 1;
inline const int STATS_IDDITEMSET          = 2;

inline const int SUBSYS_CMD                = 3;
inline const int CMD_PCJSON                = 1;
inline const int CMD_PINGFROMPC            = 2;   // ping pc to app - these are to check pc - app performance
inline const int CMD_PINGFROMPCACK         = 3;   // ping pc to app ack
inline const int CMD_PINGTOPC              = 4;   // ping app to pc
inline const int CMD_PINGTOPCACK           = 5;   // ping app to pc ack
inline const int CMD_BOTEVENT              = 6;   // diagnositc bot event

// For the following. MsgObject constructor Msg variables as follows
// subSys = SUBSYS_OBJ
// command/domain = OBJ_XXX
// objectId = OBJ_XXX_XXXX
// jsonObjectString = objectString
inline const int SUBSYS_OBJ                = 4;
inline const int OBJ_UNKNOWN                       = 0;

inline const int OBJ_LOGGER                        = 1;
inline const int OBJ_LOGGER_LOGLINE                        = 1;
inline const int OBJ_LOGGER_STATELIST                      = 2;
inline const int OBJ_LOGGER_COMMAND_TO_MATRIX              = 3;
inline const int OBJ_LOGGER_JCOMMAND_REQUEST_LOGS               = 1;
inline const int OBJ_LOGGER_JRESPONSE_FROM_MATRIX          = 4;

inline const int OBJ_APPMGR                        = 2;
inline const int OBJ_APPMGR_APPINFOLIST                     = 1;
inline const int OBJ_APPMGR_SUSPENDLIST                     = 2;
inline const int OBJ_APPMGR_COMMAND_TO_MATRIX               = 3;
inline const int OBJ_APPMGR_JCOMMAND_REQUEST_SUSPENDLIST        = 1;
inline const int OBJ_APPMGR_JCOMMAND_ADD_SUSPENDLISTITEMS       = 2;
inline const int OBJ_APPMGR_JCOMMAND_DEL_SUSPENDLISTITEMS       = 3;
inline const int OBJ_APPMGR_RESPONSE_FROM_MATRIX            = 4;
inline const int OBJ_APPMGR_JRESPONSE_REQUEST_SUSPENDLIST       = 1;
inline const int OBJ_APPMGR_JRESPONSE_ADD_SUSPENDLISTITEMS      = 2;
inline const int OBJ_APPMGR_JRESPONSE_DEL_SUSPENDLISTITEMS      = 3;

inline const int OBJ_STATSMGR                       = 3;
inline const int OBJ_STATSMGR_COMMAND_TO_MARTRIX            = 1;
inline const int OBJ_STATSMGR_JCOMMAND_QUERY_STATSSUMMARY       = 1;
inline const int OBJ_STATSMGR_RESPONSE_FROM_MATRIX          = 2;
inline const int OBJ_STATSMGR_JRESPONSE_QUERY_STATSSUMMARY_GENERAL   = 1;
inline const int OBJ_STATSMGR_JRESPONSE_QUERY_STATSSUMMARY_APPSTATS  = 2;



// Base message class. This can be moved out of here if it needs to be specific to each
// Language. For the moment it seems it can be common.

class Msg {
public:
    short int prot = 0;             // this should alway be set to the PROTOCOL_CODE
    short int length = 0;           // this will be updated by the SerDes class on calls to length()
    short int deviceAppKey = 0;     // a device and app identifier
    short int sessionKey = 0;    // a session number / key
    int seqNumber = 0;              // sequence number within the session
    short int subSys = 0;           // to be set by derived classes
    short int command = 0;          // to be set by derived classes
    int argument = 0;               // optional command argument / data
    int localParam1 = 0;            // Local parameter is NOT SERIALIZED. Used to embed local data
    short int crc = 0;              // header crc
    Msg() {
        prot = PROTOCOL_CODE;
        length = 0;
        deviceAppKey = 0;
        sessionKey = 0;
        seqNumber = 0;
        subSys = 0;
        command = 0;
        argument = 0;
        crc = 0;
    }
    M_CPPONLY(virtual ~Msg() {})
    virtual int serialize(RSerDes sd) {
        sd.setInt16(prot);
        sd.setLength16(length);
        sd.setInt16(deviceAppKey);
        sd.setInt16(sessionKey);
        sd.setInt32(seqNumber);
        sd.setInt16(subSys);
        sd.setInt16(command);
        sd.setInt32(argument);
        sd.setCrc16(0);

        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
    }
    virtual int deserialize(RSerDes sd) {
        prot = sd.getProtocolCodeAndCheckEndian(PROTOCOL_CODE);
        length = sd.getInt16();
        deviceAppKey = sd.getInt16();
        sessionKey = sd.getInt16();
        seqNumber = sd.getInt32();
        subSys = sd.getInt16();
        command = sd.getInt16();
        argument = sd.getInt32();
        crc = sd.getInt16();
        return sd.length();
    }
    short int calcCrc() {
        short int crc = prot ^ length ^ subSys ^ command ^ seqNumber ^ sessionKey ^ deviceAppKey;
        return crc;
    }
};

class MsgPCInfo :public Msg {
public:
    int64_t uid = 0;
    String name = "";
    String ipAddress = "";
    String ipGateway = "";
    int port = 0;
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

        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
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
    short int statId = 0;
    short int groupId = 0;
    short int streamId = 0;
    int value = 0;
    int time = 0;
    MsgIddStatItem() {
        value = 0;
        time = 0;
        statId = 0;
        groupId = 0;
        streamId = 0;
    }
    void serialize(RSerDes sd) {
        sd.setInt16(statId);
        sd.setInt16(groupId);
        sd.setInt16(streamId);
        sd.setInt32(value);
        sd.setInt32(time);
    }
    void deserialize(RSerDes sd) {
        statId = sd.getInt16();
        groupId = sd.getInt16();
        streamId = sd.getInt16();
        value = sd.getInt32();
        time = sd.getInt32();
    }
};

class MsgIddStatItemSet :public Msg {
public:
    String name = "";
    String description = "";
    int id = 0;
    int listLength = 0;
    List<MsgIddStatItem> statList;
    MsgIddStatItemSet() {
        subSys = SUBSYS_STATS;
        command = STATS_IDDITEMSET;
        name = "";
        description = "";
        id = 0;
        listLength = 0;
//        M_ALLOCATELIST(MsgIddStatItem,statList)
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
        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
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
    String jsonStatInfoString = "";
    MsgStatInfo() {
        subSys = SUBSYS_STATS;
        command = STATS_STATINFO;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(jsonStatInfoString);
        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
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
    String jsonCmdString = "";
    MsgCmd(String cmdString) {
        subSys = SUBSYS_CMD;
        command = CMD_PCJSON;
        jsonCmdString = cmdString;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(jsonCmdString);
        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        jsonCmdString = sd.getString();
        return sd.length();
    }
};


class MsgObject :public Msg {
public:
    String jsonObjectString = "";
    int objectId = 0;
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
        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        objectId = sd.getInt32();
        jsonObjectString = sd.getString();
        return sd.length();
    }
};
