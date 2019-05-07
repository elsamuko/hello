#!/usr/bin/env bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
MAIN_DIR="$SCRIPT_DIR/.."
cd "$MAIN_DIR" || exit

KAITAI="kaitai-struct-compiler"
SNAPSHOT="${KAITAI}-0.9-SNAPSHOT522603588"
ZIPFILE="${SNAPSHOT}.zip"
URL="https://dl.bintray.com/kaitai-io/universal_unstable/$ZIPFILE"

# remove old folders
if [ -d "$KAITAI" ]; then
    rm -rf "$KAITAI"
fi

if [ -d "$SNAPSHOT" ]; then
    rm -rf "$SNAPSHOT"
fi

# download zip
if [ ! -f "$ZIPFILE" ]; then
    curl -Ls "$URL" -o "$ZIPFILE"
fi

# extract and rename folder
unzip "$ZIPFILE"
mv "$SNAPSHOT" "$KAITAI"
