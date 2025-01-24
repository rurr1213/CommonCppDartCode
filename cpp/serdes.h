#pragma once

#include <stdlib.h>
#include <assert.h>
#include <string>

const int ENDIAN_LITTLE = 0;
const int ENDIAN_BIG = 1;

class SerDes {
    bool m_badMessageDontDeserialize;
    int m_useEndian;    // endianness to use
    char* m_pdataOrig;  // original data start position
    char* m_pdata;      // working data pointer
    int m_maxSize;      // size of original data
    int m_usedSize;     // size of used data
    char* m_plengthPos; // position of length variable
    char* m_pCrcPos;    // position of crc variable
    char* m_pMsgCrcPos; // position of Msg crc variable
public:
    SerDes(const char* _pdata, int maxSize);
    void init(const char* _pdata, int maxSize);
    int getProtocolCodeAndCheckEndian(int PROTOCOL_CODE);
    void _swapEndian();
    void _goBackNumBytes(int numBytes);
    void _setBadMessageDontDeserialize(void);
    int setLength32(int& value);
    void updateLength32(int& value);
    int setCrc16(short int value);
    void updateCrc16(short int& value);
    int setMsgCrc16(short int value);
    void updateMsgCrc16(short int& value);
    int setInt16(short int& value);
    int setInt32(int& value);
    int setInt64(int64_t& value);
    int setString(std::string stringValue);
    short int getInt16(void);
    int getInt32(void);
    long long getInt64(void);
    std::string getString(void);
    int length(void);
    int updateLength(void);
    void updateCrc(short int crc);
    void updateMsgCrc(short int crc);
    int finalize(void);
};

