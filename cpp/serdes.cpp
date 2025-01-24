#include <list> 
#include <iostream>

#include "serdes.h"

#ifdef _WIN64
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <string.h>
#include <cstring>
#define htonll htobe64
#define ntohll be64toh
using namespace std;
#endif

SerDes::SerDes(const char* _pdata, const int maxSize) {
    init(_pdata, maxSize);
}

void SerDes::init(const char* _pdata, const int maxSize) {
    m_pdataOrig = (char*)_pdata;
    m_pdata = (char*)_pdata;
    m_maxSize = maxSize;
    m_usedSize = 0;
    m_useEndian = ENDIAN_BIG;
    m_badMessageDontDeserialize = false;
    m_plengthPos = 0;
    m_pCrcPos = 0;
    m_pMsgCrcPos = 0;
}

int SerDes::getProtocolCodeAndCheckEndian(int _protocolCode) {
    int protocolCode = getInt16();
    if (protocolCode != _protocolCode) {
        //swap endianess and try again
        _swapEndian();
        _goBackNumBytes(2);
        protocolCode = getInt16();
        if (protocolCode != _protocolCode) {
            _setBadMessageDontDeserialize();
        }
    }
    return protocolCode;
}

void SerDes::_swapEndian() {
    if (m_useEndian == ENDIAN_BIG) m_useEndian = ENDIAN_LITTLE;
    else m_useEndian = ENDIAN_BIG;
}

void SerDes::_goBackNumBytes(int numBytes) {
    m_pdata -= numBytes;
    assert(m_pdata >= m_pdataOrig);
}

void SerDes::_setBadMessageDontDeserialize(void) {
    m_badMessageDontDeserialize = true;
}

int SerDes::setLength32(int& value) {
    m_plengthPos = m_pdata; // store for later update
    return setInt32(value);
}

void SerDes::updateLength32(int& value) {
    if (m_plengthPos) {
        int netValue = value;
        if (m_useEndian == ENDIAN_BIG) netValue = htonl(value);
        memcpy(m_plengthPos, &netValue, 4);
    }
}

int SerDes::setCrc16(short int value) {
    m_pCrcPos = m_pdata; // store for later update
    return setInt16(value);
}

int SerDes::setMsgCrc16(short int value) {
    m_pMsgCrcPos = m_pdata; // store for later update
    return setInt16(value);
}

void SerDes::updateCrc16(short int& value) {
    if (m_pCrcPos) {
        short int netValue = value;
        if (m_useEndian == ENDIAN_BIG) netValue = htons(value);
        memcpy(m_pCrcPos, &netValue, 2);
    }
}

void SerDes::updateMsgCrc16(short int& value) {
    if (m_pMsgCrcPos) {
        short int netValue = value;
        if (m_useEndian == ENDIAN_BIG) netValue = htons(value);
        memcpy(m_pMsgCrcPos, &netValue, 2);
    }
}

int SerDes::setInt16(short int& value) {
    int size = 2;
    assert(m_usedSize + size <= m_maxSize);    // check for space
    short int netValue = value;
    if (m_useEndian==ENDIAN_BIG) netValue = htons(value);
    memcpy(m_pdata, &netValue, size);
    m_pdata += size;
    m_usedSize += size;
    return size;
}

int SerDes::setInt32(int& value) {
    int size = 4;
    assert(m_usedSize + size <= m_maxSize);    // check for space
    int netValue = value;
    if (m_useEndian == ENDIAN_BIG) netValue = htonl(value);
    memcpy(m_pdata, &netValue, size);
    m_pdata += size;
    m_usedSize += size;
    return size;
}

int SerDes::setInt64(int64_t& value) {
    int size = 8;
    assert(m_usedSize + size <= m_maxSize);    // check for space
    int64_t netValue = value;
    if (m_useEndian == ENDIAN_BIG) netValue = htonll(value);
    memcpy(m_pdata, &netValue, size);
    m_pdata += size;
    m_usedSize += size;
    return size;
}

int SerDes::setString(std::string stringValue) {
    int size = (int)strlen(stringValue.c_str());
//    int size = (int)stringValue.size();
    assert(m_usedSize + size <= m_maxSize);    // check for space
    memcpy(m_pdata, stringValue.c_str(), size);
    m_pdata += size;
    *m_pdata++ = 0; // null terminate
    m_usedSize += size + 1;
    return size;
}

short int SerDes::getInt16(void) {
    if (m_badMessageDontDeserialize) return 0;
    int size = 2;
    int value = 0;
    assert(m_usedSize + size <= m_maxSize);    // check for valid data
    memcpy(&value, m_pdata, size);
    int hostValue = value;
    if (m_useEndian == ENDIAN_BIG) hostValue = ntohs(value);
    m_pdata += size;
    m_usedSize += size;
    return hostValue;
}

int SerDes::getInt32(void) {
    if (m_badMessageDontDeserialize) return 0;
    int size = 4;
    int value = 0;
    assert(m_usedSize + size <= m_maxSize);    // check for valid data
    memcpy(&value, m_pdata, size);
    int hostValue = value;
    if (m_useEndian == ENDIAN_BIG) hostValue = ntohl(value);
    m_pdata += size;
    m_usedSize += size;
    return hostValue;
}

long long SerDes::getInt64(void) {
    if (m_badMessageDontDeserialize) return 0;
    int size = 8;
    long long value = 0;
    assert(m_usedSize + size <= m_maxSize);    // check for valid data
    memcpy(&value, m_pdata, size);
    long long hostValue = value;
    if (m_useEndian == ENDIAN_BIG) hostValue = ntohll(value);
    m_pdata += size;
    m_usedSize += size;
    return hostValue;
}


std::string SerDes::getString(void) {
    std::string stringValue;
    if (m_badMessageDontDeserialize) return stringValue;
    stringValue = m_pdata;
    int size = (int)stringValue.size();
    m_pdata += ((int64_t)size) + 1; // increment past null terminator
    m_usedSize += size + 1;
    return stringValue;
}

int SerDes::length(void) {
    updateLength32((int&)m_usedSize);  // update the length value in the message
    return m_usedSize;
}

int SerDes::updateLength(void) {
    updateLength32((int&)m_usedSize);  // update the length value in the message
    return m_usedSize;
}

void SerDes::updateCrc(short int value) {
    updateCrc16(value);
}

void SerDes::updateMsgCrc(short int value) {
    updateMsgCrc16(value);
}

int SerDes::finalize(void) {
    return length();
}

