#pragma once
#include <string>

// https://gcc.gnu.org/onlinedocs/gcc/Structure-Layout-Pragmas.html
#pragma pack(push,1)
struct TLSRecord {
    uint8_t content_type = 0x16; // 0x16
    uint16_t version = 0x0103;   // v3.1
    uint16_t length;
};
#pragma pack(pop)

namespace hello {
std::string server();
std::string client();
}
