#!/usr/bin/env bash

KAITAI_MOC=../kaitai-struct-compiler/bin/kaitai-struct-compiler
KAITAI_JAR="kaitai-struct-runtime-0.8.jar"
URL="https://repo1.maven.org/maven2/io/kaitai/kaitai-struct-runtime/0.8/$KAITAI_JAR"

if [ ! -f "$KAITAI_MOC" ]; then
    echo "kaitai compiler needed, please run download_kaitai.sh from scripts folder"
    exit 1
fi

if [ ! -f "$KAITAI_JAR" ]; then
    echo "Downloading $KAITAI_JAR"
    curl -sO "$URL"
fi

if [ ! -f TlsClientHello.jar ]; then
    ./"$KAITAI_MOC" -t java --outdir . ../src/kaitai/tls_client_hello.ksy
    (cd src || exit
    javac -cp "../$KAITAI_JAR" TlsClientHello.java
    jar -cvf ../TlsClientHello.jar ./*.class
    rm ./*.class)
fi

if [ ! -f TlsRecord.jar ]; then
    ./"$KAITAI_MOC" -t java --outdir . ../src/kaitai/tls_record.ksy
    (cd src || exit
    javac -cp "../$KAITAI_JAR" TlsRecord.java
    jar -cvf ../TlsRecord.jar ./*.class
    rm ./*.class)
fi

groovy -cp "./*" Server.groovy
