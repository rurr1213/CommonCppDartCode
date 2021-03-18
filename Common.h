
/* -------------------------------------------------------------------------------------------

   This is common code tha MAY be by both the C++ compiler and then coverted to Dart code
   The code used here is carefully chosen to be easily coverted to Dart code using 
   sed and a sed script. 

   See comments in Message/MessageCommon.h

    Ravi Rajapakse

    -------------------------------------------------------------------------------------------
*/


/* The following are constants used on the PC side. This info is passed to flutter/dart embedded in messages 
    sent over. These codes do NOT NEED to be used in flutter code, except to easily override, text, without having
    to search for the corresponding text. Use with care on the flutter side.
*/
const int STATS_GROUPID_INVALID     = 0;    // invalid group
const int STATS_STATID_INVALID      = 0;

const int STATS_GROUPID_NET         = 1;
const int STATS_STATID_GATEWAY      = 1;
const int STATS_STATID_SITES        = 2;

const int STATS_GROUPID_CPU         = 2;
const int STATS_STATID_SPEED        = 1;

const int STATS_GROUPID_GPU         = 3;
const int STATS_STATID_FPS          = 1;

const int STATS_GROUPID_APP         = 4;
const int STATS_STATID_APP          = 1;
