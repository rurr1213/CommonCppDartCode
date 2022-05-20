/* -------------------------------------------------------------------------------------------

   This is HyperCube specific common code used by both the C++ compiler and then coverted to Dart code
   The code used here is carefully chosen to be easily coverted to Dart code using 
   sed and a sed script. 

   See comments in messageCommont.h, which are applicable to this file as well

   Ravi Rajapakse

    -------------------------------------------------------------------------------------------
*/

inline const int HC_APPID_VORTEX = 1000;

// -----------------------------------------------------------------------------------------------------------------

enum class CONNECTIONINFO_ACCESS {
    ANY,
    PROTECTED,
    SUPER
};

enum class HYPERCUBECOMMANDS {
    NONE,
    LOCALPING,
    REMOTEPING,
    ECHODATA,
    CONNECTIONINFO,
    CONNECTIONINFOACK,
    CREATEGROUP,
    CREATEGROUPACK,
    DESTROYGROUP,
    DESTROYGROUPACK,
    DESTROYINGGROUP,
    SUBSCRIBE,
    SUBSCRIBEACK,
    SUBSCRIBER,
    UNSUBSCRIBE,
    UNSUBSCRIBEACK,
    UNSUBSCRIBER,
    GETGROUPS,
    GETGROUPSACK,
    ALTERNATEHYPERCUBEIP,
    ALTERNATEHYPERCUBEIPACK,
    CLOSEDFORDATA
};

/*
static const std::string COMMAND_CONNECTIONINFO = "connectionInfo";
static const std::string COMMANDACK_CONNECTIONINFO = COMMAND_CONNECTIONINFO + "Ack";
static const std::string COMMAND_CREATEGROUP = "createGroup";
static const std::string COMMANDACK_CREATEGROUP = COMMANDACK_CREATEGROUP + "Ack";
static const std::string COMMAND_DESTROYGROUP = "destroyGroup";
static const std::string COMMANDACK_DESTROYGROUP = COMMAND_DESTROYGROUP + "Ack";
static const std::string COMMAND_DESTROYINGGROUP = "destroyingGroup";
static const std::string COMMAND_SUBSCRIBE = "subscribe";
static const std::string COMMANDACK_SUBSCRIBE = COMMAND_SUBSCRIBE + "Ack";
static const std::string COMMAND_UNSUBSCRIBE = "unsubscribe";
static const std::string COMMANDACK_UNSUBSCRIBE = COMMAND_UNSUBSCRIBE + "Ack";
*/

class CommonInfoBase {
    public:
    CommonInfoBase() {}
    virtual void from_json(M_JSONORDYNAMIC jsonData) { }
    virtual M_JSONORDYNAMIC to_json() { return 0; }
};

class ConnectionInfo : public CommonInfoBase {
public:
    std::string connectionName = "undefined";
    std::string systemName = "unknown";
    std::string appInstanceUUID = "noUUID";
    std::string serverIpAddress = "noset";
    CONNECTIONINFO_ACCESS access = M_ENUM(CONNECTIONINFO_ACCESS,ANY);
    ConnectionInfo() {}
    M_JSON to_json() {
        return {
            M_JSONPAIR("connectionName", connectionName),
            M_JSONPAIR("appInstanceUUID", appInstanceUUID),
            M_JSONPAIR("systemName", systemName),
            M_JSONPAIR("serverIpAddress", serverIpAddress),
            M_JSONPAIR("access", M_ENUMINDEX(access))
        };
    }
    void from_json(M_JSONORDYNAMIC jsonData) {
        connectionName = jsonData["connectionName"];
        systemName = jsonData["systemName"];
        appInstanceUUID = jsonData["appInstanceUUID"];
        serverIpAddress = jsonData["serverIpAddress"];
        access = M_INTTODARTENUM(CONNECTIONINFO_ACCESS,jsonData["access"]);
    }
    void copy(M_BYREF(ConnectionInfo,other)) {
        connectionName = other.connectionName;
        systemName = other.systemName;
        appInstanceUUID = other.appInstanceUUID;
        serverIpAddress = other.serverIpAddress;
        access = other.access;
    }
    bool hasWord(std::string searchWord) {
        bool stat = false;
        stat = M_FIND(connectionName,searchWord);
        if (stat) return stat;
        stat = M_FIND(systemName,searchWord);
        if (stat) return stat;
        stat = M_FIND(appInstanceUUID,searchWord);
        return stat;
    }
    std::string toString() {
        return connectionName;
    }
};

class ConnectionInfoAck : public ConnectionInfo 
{
    public:
    std::string alternateHyperCubeIp = "alternateHyperCubeIp";
    M_JSON to_json() {
        M_JSON jsonData = M_BASECLASS(ConnectionInfo, to_json());
        jsonData["alternateHyperCubeIp"] = alternateHyperCubeIp;
        return jsonData;
    }
    void copyConnectonInfo(M_BYREF(ConnectionInfo,other)) {
        M_BASECLASS(ConnectionInfo, copy(other));
    }
};


enum class GROUPINFO_ACCESS {
    ANY,
    PROTECTED,
    SUPER
};

class GroupInfo : public CommonInfoBase {
    public:
        std::string groupName = "none";
        GROUPINFO_ACCESS access =  M_ENUM(GROUPINFO_ACCESS,ANY);
        int maxMembers = 0;

        GroupInfo() {}

        M_JSON to_json() {
            return {
                M_JSONPAIR("groupName", groupName)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            groupName = jsonData["groupName"];
        }

        bool hasWord(std::string searchWord) {
            bool stat = M_FIND(groupName,searchWord);
            return stat;
        }

        std::string toString() { return groupName; }
};

class SubscriberInfo : public CommonInfoBase {
    public:
        std::string groupName = "none";
        GROUPINFO_ACCESS access =  M_ENUM(GROUPINFO_ACCESS,ANY);

        SubscriberInfo() {}

        M_JSON to_json() {
            return {
                M_JSONPAIR("groupName", groupName)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            groupName = jsonData["groupName"];
        }

        bool hasWord(std::string searchWord) {
            bool stat = M_FIND(groupName,searchWord);
            return stat;
        }

        std::string toString() { return groupName; }
};

class GetGroupsInfo : public CommonInfoBase {
    public:
        std::string searchWord = "";
        int startingIndex = 0;
        int maxItems = 0;

        GetGroupsInfo() {}

        M_JSON to_json() {
            return {
                M_JSONPAIR("searchWord", searchWord),
                M_JSONPAIR("startingIndex", startingIndex),
                M_JSONPAIR("maxItems", maxItems)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            searchWord = jsonData["searchWord"];
            startingIndex = jsonData["startingIndex"];
            maxItems = jsonData["maxItems"];
        }
};

class GroupsInfoList : public CommonInfoBase {
    public:
        M_DECLARELIST(List<GroupInfo>,list);

        GroupsInfoList() {}

        M_JSONL to_json() {
            M_DECLAREJSONLIST(jgroupInfoList);
            M_LISTFORLOOPSTART(item,list)
                GroupInfo groupInfo = item;
                M_JSON jgroupInfo = groupInfo.to_json();
                M_JSONPUSHBACK(jgroupInfoList,jgroupInfo);
            }            
            return jgroupInfoList;
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_LISTFORLOOPSTART(item,jsonData)
                M_DECLAREVARIABLE(GroupInfo,groupInfo);
                groupInfo.from_json(item);
                M_LISTPUSHBACK(list, groupInfo);
            }
        }
};

class AlternateHyperCubeInfo : public CommonInfoBase {
    public:
        int targetIp = 0;
        int maskIp = 0;
        std::string alternateHyperCubeIp = "";

        AlternateHyperCubeInfo() {}

        M_JSON to_json() {
            return {
                M_JSONPAIR("targetIp", targetIp),
                M_JSONPAIR("ipMask", maskIp),
                M_JSONPAIR("alternateHyperCubeIp", alternateHyperCubeIp)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            targetIp = jsonData["targetIp"];
            maskIp = jsonData["ipMask"];
            alternateHyperCubeIp = jsonData["alternateHyperCubeIp"];
        }
};


// -----------------------------------------------------------------------------------------------------------------

class HyperCubeCommand {
public:
    HYPERCUBECOMMANDS command = M_ENUM(HYPERCUBECOMMANDS,NONE);
    M_JSONORDYNAMIC jsonData;
    bool status = true;
    bool ack = false;
    HyperCubeCommand(HYPERCUBECOMMANDS _command, M_JSONORDYNAMIC _jsonData, bool _status) 
    {
        command = _command;
        jsonData = _jsonData;
        status = _status;
        ack = false;
    }
    void from_json(M_JSON _jsonData) {
        command = M_INTTODARTENUM(HYPERCUBECOMMANDS,jsonData["command"]);
        if (_jsonData["data"] != NULL) {
            jsonData = _jsonData["data"];
        }
        status = _jsonData["status"];
        ack = _jsonData["ack"];
    }
    M_JSON to_json() {
        return {
            M_JSONPAIR("command", M_ENUMINDEX(command)),
            M_JSONPAIR("data", jsonData),
            M_JSONPAIR("status", status),
            M_JSONPAIR("ack", ack)
        };
    }
    void init(HYPERCUBECOMMANDS _command, M_BYREF(CommonInfoBase,commonInfoBase), bool _status) {
        command = _command;
        jsonData = commonInfoBase.to_json();
        status = _status;
        ack = false;
    }
    void copy(HyperCubeCommand other) {
        command = other.command;
        jsonData = other.jsonData;
        status = other.status;
        ack = other.ack;
    }
};
