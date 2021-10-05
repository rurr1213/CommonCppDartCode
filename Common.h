
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
    STATS_GROUPID_INVALID,
    STATS_GROUPID_NET,
    STATS_GROUPID_CPU,
    STATS_GROUPID_GPU,
    STATS_GROUPID_APP
};

enum STATIDS {
    STATS_STATID_INVALID,
    STATS_STATID_GATEWAY,      // PING - gateway 
    STATS_STATID_SITES,     // PING - Sites
    STATS_STATID_SPEED,     // CPU Speed
    STATS_STATID_BUSY,      // CPU Busy
    STATS_STATID_FREQ,      // CPU Clock
    STATS_STATID_FPS,      // GPU FPS
    STATS_STATID_APP,

    // add new IDs here
};



