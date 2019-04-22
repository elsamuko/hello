#pragma once

#include <string>

#include "kaitai/tls_client_hello.h"

class Parser {
    public:
        Parser( const std::string& data_ ) :
            data( data_ ) {}
        bool parse();
        void dump();
        tls_client_hello_t* hello() const;
    private:
        std::string data;
        std::unique_ptr<tls_client_hello_t> parsed;
};
