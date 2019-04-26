#!/usr/bin/env bash

URL="https://www.iana.org/assignments/tls-extensiontype-values/tls-extensiontype-values-1.csv"

if [ ! -f extensions.csv ]; then
    curl -s "$URL" > extensions.csv
fi

echo "#pragma once

#include <map>

namespace extension {

static std::map<uint16_t,const char*> extensions = {"

while IFS=, read -r KEY VALUE REST
do
    # only use numbers, not ranges
    if [ "$KEY" -eq "$KEY" ] 2>/dev/null ; then
        VALUE=$(echo "$VALUE" | tr -d '"')
        # echo "[$KEY] : [$VALUE]"
        printf "    { %i, \"%s\" },\n" "$KEY" "$VALUE"
    fi
done < extensions.csv

echo "};
}
"
