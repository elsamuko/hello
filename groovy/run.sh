#!/usr/bin/env bash

KAITAI_MOC=../kaitai-struct-compiler/bin/kaitai-struct-compiler
KAITAI_JAR="$HOME/.groovy/grapes/io.kaitai/kaitai-struct-runtime/jars/kaitai-struct-runtime-0.8.jar"

if [ ! -f TlsClientHello.jar ]; then
    ./"$KAITAI_MOC" -t java --outdir . ../src/kaitai/tls_client_hello.ksy
    (cd src
    javac -cp "$KAITAI_JAR" TlsClientHello.java
    jar -cvf ../TlsClientHello.jar *.class
    rm *.class)
fi

if [ ! -f TlsRecord.jar ]; then
    ./"$KAITAI_MOC" -t java --outdir . ../src/kaitai/tls_record.ksy
    (cd src
    javac -cp "$KAITAI_JAR" TlsRecord.java
    jar -cvf ../TlsRecord.jar *.class
    rm *.class)
fi

groovy -cp TlsClientHello.jar:TlsRecord.jar:"$KAITAI_JAR" Server.groovy
