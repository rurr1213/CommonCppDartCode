#pragma

#include <uuid/uuid.h>
#include <string>

#include "UUID.h"

UUID::UUID() {
    uuid_clear(uuid);
}

void UUID::generate() {
    uuid_generate(uuid);
}

std::string UUID::toString() const {
    char str[37]; // UUID string representation is 36 characters + null terminator
    uuid_unparse(uuid, str);
    return std::string(str);
}

void UUID::fromString(const std::string &str) {
    uuid_parse(str.c_str(), uuid);
}

// Get raw 128-bit value
const uuid_t& UUID::getRaw() const {
    return uuid;
}

std::string createUUIDString(void) {
    UUID uuid;
    uuid.generate();
    return uuid.toString();
}
