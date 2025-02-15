#pragma

#include <uuid/uuid.h>
#include <string>

class UUID {
public:
    UUID();
    void generate();
    std::string toString() const;
    void fromString(const std::string &str);
    const uuid_t &getRaw() const;
private:
    uuid_t uuid; // 128-bit value
};

std::string createUUIDString(void);
