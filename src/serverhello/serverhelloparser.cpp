#include "serverhelloparser.hpp"
#include "log.hpp"

#include <iomanip>


#include <boost/endian/conversion.hpp>

// #include "ciphersuites.hpp"
// #include "extensions.hpp"

bool ServerHelloParser::parse() {
    try {
        kaitai::kstream ks( data );
        parsed = std::make_unique<tls_server_hello_t>( &ks );
    } catch( std::ios_base::failure ex ) {
        LOG( "Failed to parse : " << ex.what() );
    }

    return !!parsed;
}

void ServerHelloParser::dump() {
    if( !parsed ) {
        LOG( "parsed is null" );
        return;
    }

    // dump
    LOG( "type          : " << static_cast<int>( parsed->type() ) );
    LOG( "size          : " << static_cast<int>( parsed->size() ) );
    LOG( "major         : " << static_cast<int>( parsed->version()->major() ) );
    LOG( "minor         : " << static_cast<int>( parsed->version()->minor() ) );
}

tls_server_hello_t* ServerHelloParser::hello() const {
    if( parsed ) {
        return parsed.get();
    }

    return nullptr;
}
