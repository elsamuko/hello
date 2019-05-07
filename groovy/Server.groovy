#!/usr/bin/env groovy
// http://programmingitch.blogspot.com/2010/04/groovy-sockets-example.html

import java.net.ServerSocket
import io.kaitai.struct.ByteBufferKaitaiStream
import io.kaitai.struct.KaitaiStruct
import io.kaitai.struct.KaitaiStream
import TlsClientHello
import TlsRecord

void dump(TlsClientHello hello) {
    println "type          : $hello.type"
    println "size          : $hello.size"
    println "major         : $hello.version.major"
    println "minor         : $hello.version.minor"
    println "random TS     : ${new Date(1000 * hello.random.gmtUnixTime)}"
    println "random        : ${(hello.random.random as byte[]).encodeHex()}"
    println "session ID    : [${(hello.sessionId.sid as byte[]).encodeHex()}]"
    hello.cipherSuites.cipherSuites.each { Integer suite ->
        println String.format("    suite         : %04x", suite)
    }
    println "compression   : ${(hello.compressionMethods.compressionMethods as byte[]).encodeHex()}"
    hello.extensions.extensions.each { TlsClientHello.Extension ext ->
        println String.format("    extension     : %04x", ext.type)
        if(ext.type == 0){
            TlsClientHello.Sni sni = ext.body
            sni.serverNames.each { TlsClientHello.ServerName serverName ->
                String hostname = new String(serverName.hostName)
                println "        server name     : ${hostname}"
            }
        }
        if(ext.type == 16){
            TlsClientHello.Alpn alpn = ext.body
            alpn.alpnProtocols.each { TlsClientHello.Protocol protocol ->
                String name = new String(protocol.name)
                println "        alpn protocol     : ${name}"
            }
        }
    }
}

println "Open socket server on port 2014"
ServerSocket server = new ServerSocket(2014)

while(true) {
    server.accept { Socket socket ->
        println "Connected on $socket" 
        socket.withStreams { SocketInputStream input, SocketOutputStream output ->
            byte[] recordData = new byte[5]
            input.read(recordData)
            ByteBufferKaitaiStream ks = new ByteBufferKaitaiStream(recordData)
            TlsRecord record = new TlsRecord(ks)
            println "TlsRecord length : ${record.length()}"

            byte[] helloData = new byte[record.length()]
            input.read(helloData)
            ByteBufferKaitaiStream ks2 = new ByteBufferKaitaiStream(helloData)
            TlsClientHello hello = new TlsClientHello(ks2)
            println "TlsClientHello size: ${hello.size}"
            dump(hello)
        }
        println "Finished"
    }
}
