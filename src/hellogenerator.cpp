#include "hellogenerator.hpp"

const unsigned char server_hello_raw[] = {
    /* 00000000 */  0x02, 0x00, 0x00, 0x39, 0x03, 0x03, 0x24, 0x8e,  0x45, 0xf1, 0x85, 0xe7, 0xc5, 0x4b, 0xe0, 0xdd,
    /* 00000010 */  0xd5, 0xa2, 0x78, 0xab, 0xdb, 0xae, 0x15, 0x7c,  0x7d, 0x34, 0x10, 0x9c, 0x89, 0x40, 0x1f, 0x1f,
    /* 00000020 */  0x24, 0x1a, 0xb9, 0x85, 0xc9, 0x4b, 0x00, 0xc0,  0x30, 0x00, 0x00, 0x11, 0xff, 0x01, 0x00, 0x01,
    /* 00000030 */  0x00, 0x00, 0x0b, 0x00, 0x04, 0x03, 0x00, 0x01,  0x02, 0x00, 0x23, 0x00, 0x00,
    /* 0000003d */
};

std::string hello::server() {
    std::string serverhello( reinterpret_cast<const char*>( server_hello_raw ), sizeof( server_hello_raw ) );
    return serverhello;
}
