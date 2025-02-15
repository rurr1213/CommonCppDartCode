#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <memory>

//#define DEFAULT_PACKET_ALLOCLENGTH MAXPACKETSIZE

class Packet {
	char* pdata;
	int allocatedLength;
	int usedLength;
public:
	Packet();
	Packet(const int size);
	Packet(const Packet& other);
	Packet(Packet&& other);
	~Packet();
	void init(const int size);
	void deinit(void);
	Packet& operator=(const Packet& p);
	Packet& operator=(Packet&& other);
	int getLength(void) const { return allocatedLength;}
	int getAllocatedLength(void) const { return allocatedLength; }
	int getUsedLength(void) const { return usedLength; }
	void setUsedLength(int _usedLength) { usedLength = _usedLength; }
	char* getpData(void) const { return pdata;  }
	typedef std::unique_ptr<Packet> UniquePtr;
	typedef std::shared_ptr<Packet> SharedPtr;
	static UniquePtr create(int size = 0) { return std::make_unique<Packet>(size); }
	virtual operator std::string();
};

enum class DEVICEID {
	UNKNOWN = -200,
	ALLDEVICES = -100,
	BACKCHANNEL = -99,
	HK = -98
};

class PacketEx {
public:
	DEVICEID deviceId;
	Packet packet;
};

class RecvPacketBuilder {
public:
	class IReadDataObject {
	public:
		virtual int readData(void* pdata, int dataLen)=0;
	};
	IReadDataObject& ireadDataObject;
	int MSGLENGTH = 0;
	int defaultPacketLength = 0;
	enum class READSTATUS {
		READERROR = -1,
		MOREDATANEEDED = 0,
		NEEDEDDATAREAD = 1,
		PEERSHUTDOWN,
	};
public:
	Packet packet;
	char* porigin;
	char* poffset;
	int neededLength;
	RecvPacketBuilder(IReadDataObject& _ireadDataObject, const int _defaultPacketLength);
	void init(void);
	void deinit(void);
	void reset(void);
	bool isHeaderRead(void);
	bool isNumReadEnough(int numRead);
	Packet& getPacket(void);
	READSTATUS readPacket(Packet& packetRead);
private:
	int getUsedDataLength(void) {
		return (int)(poffset - porigin);
	}
	READSTATUS recvNeededData(void);
};

class WritePacketBuilder {
public:
	Packet packet;
	char* porigin;
	int offset;
	int defaultPacketLength = 0;

	WritePacketBuilder(const int _defaultPacketLength);

	void init(void);
	void deinit(void);
	bool addNew(Packet& _packet);
	char* getpData(void) const;
	int getLength(void) const;
	bool setNumSent(int numSent);
	bool empty(void) const;
//	void dumpToBotBaseForDbg(void);
};

typedef std::shared_ptr<Packet>  TPacketSharedPtr;

