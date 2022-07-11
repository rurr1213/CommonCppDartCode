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
    PUBLISHINFO,
    ALTERNATEHYPERCUBEIP,
    ALTERNATEHYPERCUBEIPACK,
    CLOSEDFORDATA,
    GETHYPERCUBEINFO,
    GETHYPERCUBEINFOACK,
    GETSTATUS,
    GETSTATUSACK,
    GETLOGLINES,
    GETLOGLINESACK
};

class CommonInfoBase {
    public:
    int version = 100;
    CommonInfoBase() {}
    virtual void from_json(M_JSONORDYNAMIC jsonData) {
        version = jsonData["commonInfoVersion"];
    }
    virtual M_JSONORDYNAMIC to_json() { 
        return {
            M_JSONPAIR("commonInfoVersion", version)
        }; 
    }
    virtual void copy(M_BYREF(CommonInfoBase,other)) {
        version = other.version;
    }
};

class StringInfo : public CommonInfoBase {
public:
    std::string data = "";
    StringInfo() {}
    M_JSON to_json() {
        return {
            M_BASECLASS(CommonInfoBase, to_json()),
            M_JSONPAIR("data", data)
        };
    }
    void from_json(M_JSONORDYNAMIC jsonData) {
        M_BASECLASS(CommonInfoBase, from_json(jsonData)),
        data = jsonData["data"];
    }
};

class HyperCubeInfo : public CommonInfoBase {
public:
    std::string systemName = "unknown";
    std::string serverName = "undefined";
    std::string appUUID = "undefined";
    std::string appInstallUUID = "noUUID";
    std::string serverIpAddress = "notset";
    int serverType = 1;
    HyperCubeInfo() {}
    M_JSON to_json() {
        return {
            M_BASECLASS(CommonInfoBase, to_json()),
            M_JSONPAIR("systemName", systemName),
            M_JSONPAIR("serverName", serverName),
            M_JSONPAIR("appUUID", appUUID),
            M_JSONPAIR("appInstallUUID", appInstallUUID),
            M_JSONPAIR("serverIpAddress", serverIpAddress),
            M_JSONPAIR("serverType", M_ENUMINDEX(serverType))
        };
    }
    void from_json(M_JSONORDYNAMIC jsonData) {
        M_BASECLASS(CommonInfoBase, from_json(jsonData)),
        systemName = jsonData["systemName"];
        serverName = jsonData["serverName"];
        appUUID = jsonData["appUUID"];
        appInstallUUID = jsonData["appInstallUUID"];
        serverIpAddress = jsonData["serverIpAddress"];
        serverType = jsonData["serverType"];
    }
    void copy(M_BYREF(HyperCubeInfo,other)) {
        M_BASECLASS(CommonInfoBase, copy(other)),
        serverName = other.serverName;
        appUUID = other.appUUID;
        appInstallUUID = other.appInstallUUID;
        systemName = other.systemName;
        serverIpAddress = other.serverIpAddress;
        serverType = other.serverType;
    }
    std::string toString() {
        return serverName;
    }
};

class ConnectionInfo : public CommonInfoBase {
public:
    std::string connectionName = "undefined";
    std::string appUUID = "noUUID";
    std::string appInstallUUID = "noUUID";
    std::string systemName = "unknown";
    std::string serverIpAddress = "notset";
    std::string userName = "notset";
    std::string userUUID = "noUUID";
    std::string displayName = "notSet";
    CONNECTIONINFO_ACCESS access = M_ENUM(CONNECTIONINFO_ACCESS,ANY);
    ConnectionInfo() {}
    M_JSON to_json() {
        return {
            M_BASECLASS(CommonInfoBase, to_json()),
            M_JSONPAIR("connectionName", connectionName),
            M_JSONPAIR("appUUID", appUUID),
            M_JSONPAIR("appInstallUUID", appInstallUUID),
            M_JSONPAIR("systemName", systemName),
            M_JSONPAIR("serverIpAddress", serverIpAddress),
            M_JSONPAIR("userName", userName),
            M_JSONPAIR("userUUID", userUUID),
            M_JSONPAIR("displayName", displayName),
            M_JSONPAIR("access", M_ENUMINDEX(access))
        };
    }
    void from_json(M_JSONORDYNAMIC jsonData) {
        M_BASECLASS(CommonInfoBase, from_json(jsonData)),        
        connectionName = jsonData["connectionName"];
        appUUID = jsonData["appUUID"];
        appInstallUUID = jsonData["appInstallUUID"];
        systemName = jsonData["systemName"];
        serverIpAddress = jsonData["serverIpAddress"];
        userName = jsonData["userName"];
        userUUID = jsonData["userUUID"];
        displayName = jsonData["displayName"];
        access = M_INTTODARTENUM(CONNECTIONINFO_ACCESS,jsonData["access"]);
    }
    void copy(M_BYREF(ConnectionInfo,other)) {
        M_BASECLASS(CommonInfoBase, copy(other));
        connectionName = other.connectionName;
        appUUID = other.appUUID;
        appInstallUUID = other.appInstallUUID;
        systemName = other.systemName;
        serverIpAddress = other.serverIpAddress;
        userName = other.userName;
        userUUID = other.userUUID;
        displayName = other.displayName;
        access = other.access;
    }
    bool hasWord(std::string searchWord) {
        bool stat = false;
        stat = M_FIND(connectionName,searchWord);
        if (stat) return stat;
        stat = M_FIND(appInstallUUID,searchWord);
        if (stat) return stat;
        stat = M_FIND(systemName,searchWord);
        if (stat) return stat;
        stat = M_FIND(userName, searchWord);
        if (stat) return stat;
        stat = M_FIND(userUUID, searchWord);
        if (stat) return stat;
        stat = M_FIND(displayName, searchWord);
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
        M_DECLAREVARIABLE(ConnectionInfo,creatorConnectionInfo);

        GroupInfo() {}

        M_JSON to_json() {
            return {
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("groupName", groupName),
                M_JSONPAIR("creator", creatorConnectionInfo.to_json())
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData)),
            groupName = jsonData["groupName"];
            creatorConnectionInfo.from_json(jsonData["creator"]);
        }

        bool hasWord(std::string searchWord) {
            bool stat = M_FIND(groupName,searchWord);
            if (!stat) stat = creatorConnectionInfo.hasWord(searchWord);
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
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("groupName", groupName)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData)),
            groupName = jsonData["groupName"];
        }

        bool hasWord(std::string searchWord) {
            bool stat = M_FIND(groupName,searchWord);
            return stat;
        }

        std::string toString() { return groupName; }
};

class PublishInfo : public CommonInfoBase {
public:
    std::string groupName = "";
    std::string publishData = "";

    PublishInfo() {}

    M_JSON to_json() {
        return {
            M_BASECLASS(CommonInfoBase, to_json()),
            M_JSONPAIR("groupName", groupName),
            M_JSONPAIR("publishData", publishData)
        };
    }
    void from_json(M_JSONORDYNAMIC jsonData) {
        M_BASECLASS(CommonInfoBase, from_json(jsonData)),
        groupName = jsonData["groupName"];
        publishData = jsonData["publishData "];
    }
};

class GetGroupsInfo : public CommonInfoBase {
    public:
        std::string searchWord = "";
        int startingIndex = 0;
        int maxItems = 0;

        GetGroupsInfo() {}

        M_JSON to_json() {
            return {
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("searchWord", searchWord),
                M_JSONPAIR("startingIndex", startingIndex),
                M_JSONPAIR("maxItems", maxItems)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData)),
            searchWord = jsonData["searchWord"];
            startingIndex = jsonData["startingIndex"];
            maxItems = jsonData["maxItems"];
        }
};

class GroupsInfoList : public CommonInfoBase {
    public:
        M_DECLARELIST(List<GroupInfo>,list);

        GroupsInfoList() {}

        M_JSON to_json() {
            M_DECLAREJSONLIST(jgroupInfoList);
            M_LISTFORLOOPSTART(item,list)
                GroupInfo groupInfo = item;
                M_JSON jgroupInfo = groupInfo.to_json();
                M_JSONPUSHBACK(jgroupInfoList,jgroupInfo);
            }            
            return { 
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("list", jgroupInfoList) 
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData));
            M_JSONORDYNAMIC jlist = jsonData["list"];
            M_LISTFORLOOPSTART(item,jlist)
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
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("targetIp", targetIp),
                M_JSONPAIR("ipMask", maskIp),
                M_JSONPAIR("alternateHyperCubeIp", alternateHyperCubeIp)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData)),
            targetIp = jsonData["targetIp"];
            maskIp = jsonData["ipMask"];
            alternateHyperCubeIp = jsonData["alternateHyperCubeIp"];
        }
};

class LineList : public CommonInfoBase {
    public:
        int startingIndex = 0;
        bool moreAvailable = false;

        M_DECLARELIST(List<std::string>,list);

        LineList() {}

        M_JSONL to_json() {
            M_JSON jdata;
            M_DECLAREJSONLIST(jlineList);
            M_LISTFORLOOPSTART(item,list)
                std::string line = item;
                M_JSONPUSHBACK(jlineList, line);
            }
            return {
                M_BASECLASS(CommonInfoBase, to_json()),
                M_JSONPAIR("startingIndex", startingIndex),
                M_JSONPAIR("moreAvailable", moreAvailable),
                M_JSONPAIR("lineList", jlineList)
            };
        }
        void from_json(M_JSONORDYNAMIC jsonData) {
            M_BASECLASS(CommonInfoBase, from_json(jsonData)),
            startingIndex = jsonData["startingIndex"];
            moreAvailable = jsonData["moreAvailable"];
            M_JSON jlist = jsonData["lineList"];
            M_LISTFORLOOPSTART(item,jlist)
                M_DECLAREVARIABLE(std::string,line);
                line = item;
                M_LISTPUSHBACK(list, line);
            }
        }
};


// -----------------------------------------------------------------------------------------------------------------

class HyperCubeCommand {
    M_JSON _jsonData = M_JSONNULL;
    int version = 100;
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
    void from_json(M_JSON __jsonData) {
        version = __jsonData["version"];
        int _command = __jsonData["command"];
        command = M_INTTODARTENUM(HYPERCUBECOMMANDS,_command);
        if (__jsonData["data"] != NULL) {
            jsonData = __jsonData["data"];
        }
        status = __jsonData["status"];
        ack = __jsonData["ack"];
    }
    M_JSON to_json() {
        return {
            M_JSONPAIR("version", version),
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
        version = other.version;
        command = other.command;
        jsonData = other.jsonData;
        status = other.status;
        ack = other.ack;
    }
    M_JSONORDYNAMIC getJsonData() {
        return jsonData;
    }
};
