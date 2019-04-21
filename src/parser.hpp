#pragma once

#include <string>

#include "kaitai/tls_client_hello.h"

class Parser {
    public:
        Parser( const std::string& data_ ) :
            data( data_ ) {}
        void parse();
        void dump();
    private:
        std::string data;
        std::unique_ptr<tls_client_hello_t> parsed;
};
