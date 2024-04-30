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

#include M_SERDES

inline const int PROTOCOL_CODE = 1122;

inline const int SUBSYS_SIG                = 10;  // route signalling system
inline const int SIG_JSON                  = 1;

inline const int SUBSYS_DISCOVERY          = 20;
inline const int DISCOVERY_HELLO           = 1;
inline const int DISCOVERY_HELLOACK        = 2;   // not used
inline const int DISCOVERY_CLOSESOCKET     = 3;   // notify other end that connection is to be ended

inline const int SUBSYS_CMD                = 40;
inline const int CMD_JSON                  = 1;
inline const int CMD_PCJSON                = 11;
inline const int CMD_PINGFROMPC            = 12;   // ping pc to app - these are to check pc - app performance
inline const int CMD_PINGFROMPCACK         = 13;   // ping pc to app ack
inline const int CMD_PINGTOPC              = 14;   // ping app to pc
inline const int CMD_PINGTOPCACK           = 15;   // ping app to pc ack
inline const int CMD_BOTEVENT              = 16;   // diagnositc bot event

inline const int SUBSYS_STATS              = 42;
inline const int STATS_STATINFO            = 1;
inline const int STATS_IDDITEMSET          = 2;


// For the following. MsgObject constructor Msg variables as follows
// subSys = SUBSYS_OBJ
// command/domain = OBJ_XXX
// objectId = OBJ_XXX_XXXX
// jsonObjectString = objectString

inline const int SUBSYS_OBJ                = 44;
inline const int OBJ_UNKNOWN                       = 0;

inline const int OBJ_MATRIXMGR                     = 1;
inline const int OBJ_MATRIXMGR_COMMAND_TO_MATRIX            = 1;
inline const int OBJ_MATRIXMGR_JCOMMAND_ECHO_COMMAND            = 1;
inline const int OBJ_MATRIXMGR_RESPONSE_FROM_MATRIX         = 2;
inline const int OBJ_MATRIXMGR_JRESPONSE_ECHO_ACK               = 1;

inline const int OBJ_LOGGER                        = 2;
inline const int OBJ_LOGGER_LOGLINE                        = 1;
inline const int OBJ_LOGGER_STATELIST                      = 2;
inline const int OBJ_LOGGER_COMMAND_TO_MATRIX              = 3;
inline const int OBJ_LOGGER_JCOMMAND_REQUEST_LOGS               = 1;
inline const int OBJ_LOGGER_JCOMMAND_DELETE_LOGS                = 2;
inline const int OBJ_LOGGER_JRESPONSE_FROM_MATRIX          = 4;
inline const int OBJ_LOGGER_JRESPONSE_DELETED_LOGS = 2;

inline const int OBJ_APPMGR                        = 3;
inline const int OBJ_APPMGR_APPINFOLIST                     = 1;
inline const int OBJ_APPMGR_SUSPENDLIST                     = 2;
inline const int OBJ_APPMGR_COMMAND_TO_MATRIX               = 3;
inline const int OBJ_APPMGR_JCOMMAND_REQUEST_SUSPENDLIST        = 1;
inline const int OBJ_APPMGR_JCOMMAND_ADD_SUSPENDLISTITEMS       = 2;
inline const int OBJ_APPMGR_JCOMMAND_DEL_SUSPENDLISTITEMS       = 3;
inline const int OBJ_APPMGR_JCOMMAND_REPLACE_SUSPENDLISTITEMS   = 4;
//inline const int OBJ_APPMGR_JCOMMAND_SUSPEND_GETSTATUS          = 5;
//inline const int OBJ_APPMGR_JCOMMAND_SUSPEND_ON_OFF             = 6;
inline const int OBJ_APPMGR_JCOMMAND_CLEARALL_SUSPENDLISTITEMS  = 7;
inline const int OBJ_APPMGR_JCOMMAND_UPDATE_APPLISTNOW          = 8;

inline const int OBJ_APPMGR_RESPONSE_FROM_MATRIX            = 4;
inline const int OBJ_APPMGR_JRESPONSE_REQUEST_SUSPENDLIST       = 1;
inline const int OBJ_APPMGR_JRESPONSE_ADD_SUSPENDLISTITEMS      = 2;
inline const int OBJ_APPMGR_JRESPONSE_DEL_SUSPENDLISTITEMS      = 3;
inline const int OBJ_APPMGR_JRESPONSE_REPLACE_SUSPENDLISTITEMS  = 4;
inline const int OBJ_APPMGR_JRESPONSE_SUSPEND_GETSTATUS         = 5;
inline const int OBJ_APPMGR_JRESPONSE_SUSPEND_ON_OFF            = 6;
inline const int OBJ_APPMGR_JRESPONSE_CLEARALL_SUSPENDLISTITEMS = 7;
inline const int OBJ_APPMGR_JRESPONSE_UPDATE_APPLISTNOW         = 8;

inline const int OBJ_STATSMGR                       = 4;
inline const int OBJ_STATSMGR_COMMAND_TO_MATRIX            = 1;
inline const int OBJ_STATSMGR_JCOMMAND_QUERY_STAT               = 1;
inline const int OBJ_STATSMGR_JCOMMAND_SET_STAT                 = 2;
inline const int OBJ_STATSMGR_JCOMMAND_DEL_STAT                 = 3;
inline const int OBJ_STATSMGR_JCOMMAND_FPS_GETSTATUS            = 4;
inline const int OBJ_STATSMGR_JCOMMAND_FPS_ON_OFF               = 5;
inline const int OBJ_STATSMGR_JCOMMAND_FPS_GETINFO              = 6;

inline const int OBJ_STATSMGR_RESPONSE_FROM_MATRIX         = 2;
inline const int OBJ_STATSMGR_JRESPONSE_QUERY_GENERALSTAT       = 1;
inline const int OBJ_STATSMGR_JRESPONSE_QUERY_APPSTAT           = 2;
inline const int OBJ_STATSMGR_JRESPONSE_SET_GENERALSTAT         = 3;
inline const int OBJ_STATSMGR_JRESPONSE_SET_APPSTAT             = 4;
inline const int OBJ_STATSMGR_JRESPONSE_DEL_GENERALSTAT         = 5;
inline const int OBJ_STATSMGR_JRESPONSE_DEL_APPSTAT             = 6;
inline const int OBJ_STATSMGR_JRESPONSE_FPS_GETSTATUS           = 7;
inline const int OBJ_STATSMGR_JRESPONSE_FPS_ON_OFF              = 8;
inline const int OBJ_STATSMGR_JRESPONSE_FPS_GETINFO             = 9;



// Base message class. This can be moved out of here if it needs to be specific to each
// Language. For the moment it seems it can be common.

class Msg {
public:
    short int prot = 0;             // this should alway be set to the PROTOCOL_CODE
    int length = 0;           // this will be updated by the SerDes class on calls to length()
    short int subSys = 0;           // to be set by derived classes
    short int command = 0;          // to be set by derived classes
    int argument = 0;               // optional command argument / data
    short int deviceAppKey = 0;     // a device and app identifier
    short int sessionKey = 0;    // a session number / key
    int seqNumber = 0;              // sequence number within the session
    int localParam1 = 0;            // Local parameter is NOT SERIALIZED. Used to embed local data
    short int crc = 0;              // header crc
    Msg() {
        prot = PROTOCOL_CODE;
        length = 0;
        subSys = 0;
        command = 0;
        argument = 0;
        deviceAppKey = 0;
        sessionKey = 0;
        seqNumber = 0;
        crc = 0;
    }
    M_CPPONLY(virtual ~Msg() {})
    virtual int serialize(RSerDes sd) {
        sd.setInt16(prot);
        sd.setLength32(length);
        sd.setInt16(subSys);
        sd.setInt16(command);
        sd.setInt32(argument);
        sd.setInt16(deviceAppKey);
        sd.setInt16(sessionKey);
        sd.setInt32(seqNumber);
        sd.setCrc16(0);

        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
    }
    virtual int deserialize(RSerDes sd) {
        prot = sd.getProtocolCodeAndCheckEndian(PROTOCOL_CODE);
        length = sd.getInt32();
        subSys = sd.getInt16();
        command = sd.getInt16();
        argument = sd.getInt32();
        deviceAppKey = sd.getInt16();
        sessionKey = sd.getInt16();
        seqNumber = sd.getInt32();
        crc = sd.getInt16();
        return sd.length();
    }
    virtual short int calcCrc() {
        int crc = prot ^ length ^ subSys ^ command ^ seqNumber ^ sessionKey ^ deviceAppKey;
        crc &= 0x7FF;
        return crc;
    }

    virtual int size() { return 24; }     // bytes

    bool copy(Msg otherMsg) {
      prot = otherMsg.prot;
      length = otherMsg.length;
      subSys = otherMsg.subSys;
      command = otherMsg.command;
      argument = otherMsg.argument;
      deviceAppKey = otherMsg.deviceAppKey;
      sessionKey = otherMsg.sessionKey;
      seqNumber = otherMsg.seqNumber;
      localParam1 = otherMsg.localParam1;
      crc = otherMsg.crc;
      return true;
    }

    // This may be used by derived classes that have string data
    virtual short int calcCrcOnString(String dataString) {
        short int _dataCrc = 0;
        for (int i = 0; i < M_LISTLEN(dataString); i++) {
            _dataCrc ^= M_ELEMENT(dataString, i);
        }
        return _dataCrc;
    }
};

class MsgJson :public Msg {
public:
    String jsonData = "";
    MsgJson() {
        subSys = 0;
        command = 0;
        jsonData = "";
    }
    void set(short int _subSys, short int _command, String _jsonData) {
        subSys = _subSys;
        command = _command;
        jsonData = _jsonData;
    }
    int size() {
        int _size = M_BASECLASS(Msg, size());
        _size += M_SIZE(jsonData) + 1;
        return _size;
    }
    int serialize(RSerDes sd) {
        M_BASECLASS(Msg, serialize(sd));
        sd.setString(jsonData);
        length = sd.updateLength();
        sd.updateCrc(calcCrc());
        return sd.finalize();
    }

    int deserialize(RSerDes sd) {
        M_BASECLASS(Msg, deserialize(sd));
        jsonData = sd.getString();
        return sd.length();
    }
    virtual short int calcCrc() {
        short int _headerCrc = M_BASECLASS(Msg, calcCrc());
        short int _dataCrc = M_BASECLASS(Msg, calcCrcOnString(jsonData));
//        M_LISTFORLOOPSTART(character, jsonData)
//            _stringCrc ^= character;
//        }
        return _headerCrc ^ _dataCrc;
    }
};

class SigMsg :public MsgJson {
public:
    SigMsg(String _jsonMsg) {
        set(SUBSYS_SIG, SIG_JSON, _jsonMsg);
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
    virtual int size() {
        int _size = M_BASECLASS(Msg, size());
        _size += 8;
        _size += M_SIZE(name) + 1;
        _size += M_SIZE(ipAddress) + 1;
        _size += M_SIZE(ipGateway) + 1;
        _size += 4;
        return _size;
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
    virtual int size() {
        int _size = 0;
        _size += 2 * 3;
        _size += 4 * 2;
        return _size;
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
	}
    int size() {
        int _size = M_BASECLASS(Msg, size());
        _size += M_SIZE(name) + 1;
        _size += M_SIZE(description) + 1;
        _size += 4 * 2;
        M_LISTFORLOOPSTART(statItem, statList)
            _size += statItem.size();
        }
        return _size;
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
    int size() {
        int _size = M_BASECLASS(Msg, size());
        _size += M_SIZE(jsonStatInfoString) + 1;
        return _size;
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

class MsgCmd :public MsgJson {
public:
    MsgCmd(String _jsonSubCmd) {
        set(SUBSYS_CMD, CMD_PCJSON, _jsonSubCmd);
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
    int size() {
        int _size = M_BASECLASS(Msg, size());
        _size += M_SIZE(jsonObjectString) + 1;
        _size += 4;
        return _size;
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
    virtual short int calcCrc() {
        short int _headerCrc = M_BASECLASS(Msg, calcCrc());
        short int _dataCrc = M_BASECLASS(Msg, calcCrcOnString(jsonObjectString));
        _dataCrc ^= objectId;
        return _headerCrc ^ _dataCrc;
    }
};


