
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

const int SUBSYS_DISCOVERY = 1;
const int DISCOVERY_HELLO = 1;

const int PROTOCOL_CODE = 1122;

class Msg {
public:
    short int prot;
    short int length;
    short int subSys;
    short int command;
    Msg() {
        prot = PROTOCOL_CODE;
    }
    int serialize(SerDes& sd) {
        sd.setInt16(prot);
        sd.setLength16(length);
        sd.setInt16(subSys);
        sd.setInt16(command);
        return sd.length();
    }
    int deserialize(SerDes& sd) {
        prot = sd.getProtocolCodeAndCheckEndian(PROTOCOL_CODE);
        length = sd.getInt16();
        subSys = sd.getInt16();
        command = sd.getInt16();
        return sd.length();
    }
};

class PCInfo :public Msg {
public:
    int uid;
    std::string ipAddress;
    std::string name;
    int serialize(SerDes& sd) {
        Msg::serialize(sd);
        sd.setInt32(uid);
        sd.setString(ipAddress);
        sd.setString(name);
        return sd.length();
    }
    int deserialize(SerDes& sd) {
	Msg::deserialize(sd);
        uid = sd.getInt32();
        ipAddress = sd.getString();
        name = sd.getString();
        return sd.length();
    }
};

class MsgDiscoveryMulticast : public PCInfo {
public:
    MsgDiscoveryMulticast() {
        subSys = SUBSYS_DISCOVERY;
        command = DISCOVERY_HELLO;
    }
};


