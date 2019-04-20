#!/usr/bin/env bash

echo "
#include <map>
#include <array>

uint16_t fromUint8_t( const std::array<uint8_t,2>& arr ) {
    uint16_t rv = 256 * arr[0] + arr[1];
    return rv;
}

namespace ciphersuite {
static std::map<uint16_t,const char*> ciphers = {"

# printf "    { fromUint8_t( { %s } ), \"%s\" },\n" "0xFF,0x00" "EXTRA"

openssl ciphers -V | awk '{
    printf "    { fromUint8_t( { %s } ), \"%s\" },\n", $1, $3
}'

echo "};
}
"
