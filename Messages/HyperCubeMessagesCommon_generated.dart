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
}

class CommonInfoBase {
    
    int version = 100;
    CommonInfoBase();
    fromJson(Map<String, dynamic> jsonData) {
        version = jsonData["commonInfoVersion"];
    }
    Map<String, dynamic> toJson() { 
        return {
            "commonInfoVersion": version
        }; 
    }
    updateJson(Map<String, dynamic> jsonData) { 
        jsonData["commonInfoVersion"] = version;
    }
    copyBase(CommonInfoBase other) {
        version = other.version;
    }
}

class StringInfo extends CommonInfoBase {

    String data = "";
    StringInfo();
    Map<String, dynamic> toJson() {
        Map<String, dynamic> jdata = {
            "data": data
        };
        super. updateJson(jdata);
        return jdata;
    }
    fromJson(dynamic jsonData) {
        super. fromJson(jsonData);
        data = jsonData["data"];
    }
}

class HyperCubeInfo extends CommonInfoBase {

    String systemName = "unknown";
    String serverName = "undefined";
    String appUUID = "undefined";
    String appInstallUUID = "noUUID";
    String serverIpAddress = "notset";
    int serverType = 1;
    HyperCubeInfo();
    Map<String, dynamic> toJson() {
        Map<String, dynamic> jdata = {
            "systemName": systemName,
            "serverName": serverName,
            "appUUID": appUUID,
            "appInstallUUID": appInstallUUID,
            "serverIpAddress": serverIpAddress,
            "serverType": serverType
        };
        super. updateJson(jdata);
        return jdata;
    }
    fromJson(dynamic jsonData) {
        super. fromJson(jsonData);
        systemName = jsonData["systemName"];
        serverName = jsonData["serverName"];
        appUUID = jsonData["appUUID"];
        appInstallUUID = jsonData["appInstallUUID"];
        serverIpAddress = jsonData["serverIpAddress"];
        serverType = jsonData["serverType"];
    }
    copy(HyperCubeInfo other) {
        super. copyBase(other);
        serverName = other.serverName;
        appUUID = other.appUUID;
        appInstallUUID = other.appInstallUUID;
        systemName = other.systemName;
        serverIpAddress = other.serverIpAddress;
        serverType = other.serverType;
    }
    String toString() {
        return serverName;
    }
}

class ConnectionInfo extends CommonInfoBase {

    String connectionName = "undefined";
    String appUUID = "noUUID";
    String appInstallUUID = "noUUID";
    String systemName = "unknown";
    String serverIpAddress = "notset";
    String userName = "notset";
    String userUUID = "noUUID";
    String displayName = "notSet";
    CONNECTIONINFO_ACCESS access = CONNECTIONINFO_ACCESS.ANY;
    ConnectionInfo();
    Map<String, dynamic> toJson() {
        Map<String, dynamic> jbase = super. toJson();
        Map<String, dynamic> jdata = {
            "connectionName": connectionName,
            "appUUID": appUUID,
            "appInstallUUID": appInstallUUID,
            "systemName": systemName,
            "serverIpAddress": serverIpAddress,
            "userName": userName,
            "userUUID": userUUID,
            "displayName": displayName,
            "access": access.index
        };
        super. updateJson(jdata);
        return jdata;
    }
    fromJson(dynamic jsonData) {
        super. fromJson(jsonData);        
        connectionName = jsonData["connectionName"];
        appUUID = jsonData["appUUID"];
        appInstallUUID = jsonData["appInstallUUID"];
        systemName = jsonData["systemName"];
        serverIpAddress = jsonData["serverIpAddress"];
        userName = jsonData["userName"];
        userUUID = jsonData["userUUID"];
        displayName = jsonData["displayName"];
        access = CONNECTIONINFO_ACCESS.values[jsonData["access"]];
    }
    copy(ConnectionInfo other) {
        super. copyBase(other);
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
    bool hasWord(String searchWord) {
        bool stat = false;
        stat = connectionName.contains(searchWord);
        if (stat) return stat;
        stat = appInstallUUID.contains(searchWord);
        if (stat) return stat;
        stat = systemName.contains(searchWord);
        if (stat) return stat;
        stat = userName.contains( searchWord);
        if (stat) return stat;
        stat = userUUID.contains( searchWord);
        if (stat) return stat;
        stat = displayName.contains( searchWord);
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
        Map<String, dynamic> jdata = super. toJson();
        jdata["alternateHyperCubeIp"] = alternateHyperCubeIp;
        return jdata;
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
        var creatorConnectionInfo = ConnectionInfo();

        GroupInfo();

        Map<String, dynamic> toJson() {
            Map<String, dynamic> jdata = {
                "groupName": groupName,
                "creator": creatorConnectionInfo.toJson()
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            groupName = jsonData["groupName"];
            creatorConnectionInfo.fromJson(jsonData["creator"]);
        }

        bool hasWord(String searchWord) {
            bool stat = groupName.contains(searchWord);
            if (!stat) stat = creatorConnectionInfo.hasWord(searchWord);
            return stat;
        }

        String toString() { return groupName; }
}

class SubscriberInfo extends CommonInfoBase {
    
        String groupName = "none";
        GROUPINFO_ACCESS access =  GROUPINFO_ACCESS.ANY;

        SubscriberInfo();

        Map<String, dynamic> toJson() {
            Map<String, dynamic> jdata = {
                "groupName": groupName
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            groupName = jsonData["groupName"];
        }

        bool hasWord(String searchWord) {
            bool stat = groupName.contains(searchWord);
            return stat;
        }

        String toString() { return groupName; }
}

class PublishInfo extends CommonInfoBase {

    String groupName = "";
    String publishData = "";

    PublishInfo();

    Map<String, dynamic> toJson() {
        Map<String, dynamic> jdata = {
            "groupName": groupName,
            "publishData": publishData
        };
        super. updateJson(jdata);
        return jdata;
    }
    fromJson(dynamic jsonData) {
        super. fromJson(jsonData);
        groupName = jsonData["groupName"];
        publishData = jsonData["publishData "];
    }
}

class GetGroupsInfo extends CommonInfoBase {
    
        String searchWord = "";
        int startingIndex = 0;
        int maxItems = 0;

        GetGroupsInfo();

        Map<String, dynamic> toJson() {
            Map<String, dynamic> jdata = {
                "searchWord": searchWord,
                "startingIndex": startingIndex,
                "maxItems": maxItems
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            searchWord = jsonData["searchWord"];
            startingIndex = jsonData["startingIndex"];
            maxItems = jsonData["maxItems"];
        }
}

class GroupsInfoList extends CommonInfoBase {
    
        List<GroupInfo> list = [];

        GroupsInfoList();

        Map<String, dynamic> toJson() {
            List<dynamic> jgroupInfoList = [];
            for ( var item in list ) {
                GroupInfo groupInfo = item;
                Map<String, dynamic> jgroupInfo = groupInfo.toJson();
                jgroupInfoList.add(jgroupInfo);
            }            
            Map<String, dynamic> jdata = { 
                "list": jgroupInfoList 
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            dynamic jlist = jsonData["list"];
            for ( var item in jlist ) {
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
            Map<String, dynamic> jdata = {
                "targetIp": targetIp,
                "ipMask": maskIp,
                "alternateHyperCubeIp": alternateHyperCubeIp
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            targetIp = jsonData["targetIp"];
            maskIp = jsonData["ipMask"];
            alternateHyperCubeIp = jsonData["alternateHyperCubeIp"];
        }
}

class LineList extends CommonInfoBase {
    
        int startingIndex = 0;
        bool moreAvailable = false;

        List<String> list = [];

        LineList();

        Map<String, dynamic> toJson() {
            List<dynamic> jlineList = [];
            for ( var item in list ) {
                String line = item;
                jlineList.add( line);
            }
            Map<String, dynamic> jdata = {
                "startingIndex": startingIndex,
                "moreAvailable": moreAvailable,
                "lineList": jlineList
            };
            super. updateJson(jdata);
            return jdata;
        }
        fromJson(dynamic jsonData) {
            super. fromJson(jsonData);
            startingIndex = jsonData["startingIndex"];
            moreAvailable = jsonData["moreAvailable"];
            List<dynamic> jlist = jsonData["lineList"];
            for ( var item in jlist ) {
                String line = item;
                list.add( line);
            }
        }
}


// -----------------------------------------------------------------------------------------------------------------

class HyperCubeCommand {
    Map<String, dynamic> _jsonData = Map<String, dynamic>();
    int version = 100;

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
        version = __jsonData["version"];
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
            "version": version,
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
        version = other.version;
        command = other.command;
        jsonData = other.jsonData;
        status = other.status;
        ack = other.ack;
    }
    dynamic getJsonData() {
        return jsonData;
    }
}
