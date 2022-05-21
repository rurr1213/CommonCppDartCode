/* -------------------------------------------------------------------------------------------

   This is HyperCube specific common code used by both the C++ compiler and then coverted to Dart code
   The code used here is carefully chosen to be easily coverted to Dart code using 
   sed and a sed script. 

   See comments in messageCommont.h, which are applicable to this file as well

   Ravi Rajapakse

    -------------------------------------------------------------------------------------------
*/

 const int HC_APPID_VORTEX = 1000;

// -----------------------------------------------------------------------------------------------------------------

enum CONNECTIONINFO_ACCESS {
    ANY,
    PROTECTED,
    SUPER
}

enum HYPERCUBECOMMANDS {
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
}

/*
static const String COMMAND_CONNECTIONINFO = "connectionInfo";
static const String COMMANDACK_CONNECTIONINFO = COMMAND_CONNECTIONINFO + "Ack";
static const String COMMAND_CREATEGROUP = "createGroup";
static const String COMMANDACK_CREATEGROUP = COMMANDACK_CREATEGROUP + "Ack";
static const String COMMAND_DESTROYGROUP = "destroyGroup";
static const String COMMANDACK_DESTROYGROUP = COMMAND_DESTROYGROUP + "Ack";
static const String COMMAND_DESTROYINGGROUP = "destroyingGroup";
static const String COMMAND_SUBSCRIBE = "subscribe";
static const String COMMANDACK_SUBSCRIBE = COMMAND_SUBSCRIBE + "Ack";
static const String COMMAND_UNSUBSCRIBE = "unsubscribe";
static const String COMMANDACK_UNSUBSCRIBE = COMMAND_UNSUBSCRIBE + "Ack";
*/

class CommonInfoBase {
    
    CommonInfoBase();
    fromJson(dynamic jsonData) { }
    dynamic toJson() { return 0; }
}

class ConnectionInfo extends CommonInfoBase {

    String connectionName = "undefined";
    String systemName = "unknown";
    String appInstanceUUID = "noUUID";
    String serverIpAddress = "noset";
    CONNECTIONINFO_ACCESS access = CONNECTIONINFO_ACCESS.ANY;
    ConnectionInfo();
    Map<String, dynamic> toJson() {
        return {
            "connectionName": connectionName,
            "appInstanceUUID": appInstanceUUID,
            "systemName": systemName,
            "serverIpAddress": serverIpAddress,
            "access": access.index
        };
    }
    fromJson(dynamic jsonData) {
        connectionName = jsonData["connectionName"];
        systemName = jsonData["systemName"];
        appInstanceUUID = jsonData["appInstanceUUID"];
        serverIpAddress = jsonData["serverIpAddress"];
        access = CONNECTIONINFO_ACCESS.values[jsonData["access"]];
    }
    copy(ConnectionInfo other) {
        connectionName = other.connectionName;
        systemName = other.systemName;
        appInstanceUUID = other.appInstanceUUID;
        serverIpAddress = other.serverIpAddress;
        access = other.access;
    }
    bool hasWord(String searchWord) {
        bool stat = false;
        stat = connectionName.contains(searchWord);
        if (stat) return stat;
        stat = systemName.contains(searchWord);
        if (stat) return stat;
        stat = appInstanceUUID.contains(searchWord);
        return stat;
    }
    String toString() {
        return connectionName;
    }
}

class ConnectionInfoAck extends ConnectionInfo 
{
    
    String alternateHyperCubeIp = "alternateHyperCubeIp";
    Map<String, dynamic> toJson() {
        Map<String, dynamic> jsonData = super. toJson();
        jsonData["alternateHyperCubeIp"] = alternateHyperCubeIp;
        return jsonData;
    }
    copyConnectonInfo(ConnectionInfo other) {
        super. copy(other);
    }
}


enum GROUPINFO_ACCESS {
    ANY,
    PROTECTED,
    SUPER
}

class GroupInfo extends CommonInfoBase {
    
        String groupName = "none";
        GROUPINFO_ACCESS access =  GROUPINFO_ACCESS.ANY;
        int maxMembers = 0;

        GroupInfo();

        Map<String, dynamic> toJson() {
            return {
                "groupName": groupName
            };
        }
        fromJson(dynamic jsonData) {
            groupName = jsonData["groupName"];
        }

        bool hasWord(String searchWord) {
            bool stat = groupName.contains(searchWord);
            return stat;
        }

        String toString() { return groupName; }
}

class SubscriberInfo extends CommonInfoBase {
    
        String groupName = "none";
        GROUPINFO_ACCESS access =  GROUPINFO_ACCESS.ANY;

        SubscriberInfo();

        Map<String, dynamic> toJson() {
            return {
                "groupName": groupName
            };
        }
        fromJson(dynamic jsonData) {
            groupName = jsonData["groupName"];
        }

        bool hasWord(String searchWord) {
            bool stat = groupName.contains(searchWord);
            return stat;
        }

        String toString() { return groupName; }
}

class GetGroupsInfo extends CommonInfoBase {
    
        String searchWord = "";
        int startingIndex = 0;
        int maxItems = 0;

        GetGroupsInfo();

        Map<String, dynamic> toJson() {
            return {
                "searchWord": searchWord,
                "startingIndex": startingIndex,
                "maxItems": maxItems
            };
        }
        fromJson(dynamic jsonData) {
            searchWord = jsonData["searchWord"];
            startingIndex = jsonData["startingIndex"];
            maxItems = jsonData["maxItems"];
        }
}

class GroupsInfoList extends CommonInfoBase {
    
        List<GroupInfo> list = [];

        GroupsInfoList();

        List<dynamic> toJson() {
            List<dynamic> jgroupInfoList = [];
            for ( var item in list ) {
                GroupInfo groupInfo = item;
                Map<String, dynamic> jgroupInfo = groupInfo.toJson();
                jgroupInfoList.add(jgroupInfo);
            }            
            return jgroupInfoList;
        }
        fromJson(dynamic jsonData) {
            for ( var item in jsonData ) {
                var groupInfo = GroupInfo();
                groupInfo.fromJson(item);
                list.add( groupInfo);
            }
        }
}

class AlternateHyperCubeInfo extends CommonInfoBase {
    
        int targetIp = 0;
        int maskIp = 0;
        String alternateHyperCubeIp = "";

        AlternateHyperCubeInfo();

        Map<String, dynamic> toJson() {
            return {
                "targetIp": targetIp,
                "ipMask": maskIp,
                "alternateHyperCubeIp": alternateHyperCubeIp
            };
        }
        fromJson(dynamic jsonData) {
            targetIp = jsonData["targetIp"];
            maskIp = jsonData["ipMask"];
            alternateHyperCubeIp = jsonData["alternateHyperCubeIp"];
        }
}


// -----------------------------------------------------------------------------------------------------------------

class HyperCubeCommand {
    Map<String, dynamic> _jsonData = Map<String, dynamic>();

    HYPERCUBECOMMANDS command = HYPERCUBECOMMANDS.NONE;
    dynamic jsonData;
    bool status = true;
    bool ack = false;
    HyperCubeCommand(HYPERCUBECOMMANDS _command, dynamic _jsonData, bool _status) 
    {
        command = _command;
        jsonData = _jsonData;
        status = _status;
        ack = false;
    }
    fromJson(Map<String, dynamic> __jsonData) {
        int _command = __jsonData["command"];
        command = HYPERCUBECOMMANDS.values[_command];
        if (__jsonData["data"] != null) {
            jsonData = __jsonData["data"];
        }
        status = __jsonData["status"];
        ack = __jsonData["ack"];
    }
    Map<String, dynamic> toJson() {
        return {
            "command": command.index,
            "data": jsonData,
            "status": status,
            "ack": ack
        };
    }
    init(HYPERCUBECOMMANDS _command, CommonInfoBase commonInfoBase, bool _status) {
        command = _command;
        jsonData = commonInfoBase.toJson();
        status = _status;
        ack = false;
    }
    copy(HyperCubeCommand other) {
        command = other.command;
        jsonData = other.jsonData;
        status = other.status;
        ack = other.ack;
    }
    dynamic getJsonData() {
        return jsonData;
    }
}
