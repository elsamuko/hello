#pragma once

#include <string>

#include "kaitai/tls_server_hello.h"

class ServerHelloParser {
    public:
        ServerHelloParser( const std::string& data_ ) :
            data( data_ ) {}
        bool parse();
        void dump();
        tls_server_hello_t* hello() const;
    private:
        std::string data;
        std::unique_ptr<tls_server_hello_t> parsed;
};
