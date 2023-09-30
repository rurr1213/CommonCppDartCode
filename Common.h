#pragma once
/* -------------------------------------------------------------------------------------------

   This is common code that is used by both the C++ code and the Dart compiler.
   Therefore code used has to be Compatible with both languages.

   See comments in Message/MessageCommon.h

   Ravi Rajapakse

    -------------------------------------------------------------------------------------------
*/


/* The following are constants used on the PC side and Dart side. This info is passed to flutter/dart embedded in messages 
    sent over. 

    NOTE:
    
        1) STATID need to be globally unique. There is no special significance to the values used
        2) ONLY add new IDs at the END of the list to maintain compatibility
*/

inline const int GROUPSTATID_VERSION = 1;        // This could be used and changed if there are significant changes that need to maintain compabitility with older code

inline const int COMMON_PACKETSIZE_MAX = 64 * 1000;

enum class GROUPIDS {
    INVALID,
    NET,    //
    CPU,
    GPU,
    MEM,    //
    DISK,   //
    APP
};

enum class STATIDS {
    INVALID,
    CPU_SPEED,    
    CPU_BUSY,     
    CPU_CLOCK,    
    GPU_FPS,
    GPU_BUSY,
    MEM_PAGEF,       // page faults
    MEM_COMM,        // commited bytes
    NET_TRAFFIC,     // Network transfers      
    DISK_TRANSFERS,  // Disk transfers
    PING_GATEWAY,    // PING - gateway 
    PING_SITE_1,     // PING - Sites
    PING_SITE_2,     // PING - Sites
    PING_SITE_3,     // PING - Sites
    PING_SITE_4,     // PING - Sites
    PING_SITE_5,     // PING - Sites
    APP_STATS,
};

enum class QUERY_FUNCTION 
{
    INVALID,
    AVG,
    MAX,
    MIN,
    COUNT,
    SUMMARY1
};
