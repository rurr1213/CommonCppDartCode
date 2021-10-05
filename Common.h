
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

const int GROUPSTATID_VERSION = 1;        // This could be used and changed if there are significant changes that need to maintain compabitility with older code

enum GROUPIDS {
    INVALID_GROUPID,
    NETWORKING,
    CPU,
    GPU,
    APP
};

enum STATIDS {
    INVALID_STATID,
    PING_GATEWAY,      // PING - gateway 
    PING_SITES,     // PING - Sites
    CPU_SPEED,     // CPU Speed
    CPU_BUSY,      // CPU Busy
    CPU_CLOCK,      // CPU Clock
    GPU_FPS,      // GPU FPS
    APP_STATS,

    // add new IDs here
};



