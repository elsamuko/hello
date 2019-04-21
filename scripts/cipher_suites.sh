#!/usr/bin/env bash

echo "#pragma once

#include <map>
#include <array>

inline uint16_t fromUint8_t( const std::array<uint8_t,2>& arr ) {
    uint16_t rv = 256u * arr[0] + arr[1];
    return rv;
}

namespace ciphersuite {

static std::map<uint16_t,const char*> ciphers = {"

# https://wiki.openssl.org/index.php/SSL_and_TLS_Protocols#SCSV
printf "    { fromUint8_t( { %s } ), \"%s\" },\n" "0x00,0xFF" "TLS_EMPTY_RENEGOTIATION_INFO_SCSV"
printf "    { fromUint8_t( { %s } ), \"%s\" },\n" "0x56,0x00" "TLS_FALLBACK_SCSV"

openssl ciphers -V | awk '{
    printf "    { fromUint8_t( { %s } ), \"%s\" },\n", $1, $3
}'

echo "};
}
"
