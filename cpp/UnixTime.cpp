
#include <ctime> // Add this include

#include "UnixTime.h"

uint64_t getUnixTime(void) {
    return std::time(0);
}