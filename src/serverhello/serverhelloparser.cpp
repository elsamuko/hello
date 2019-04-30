#include "serverhelloparser.hpp"

#include <boost/endian/conversion.hpp>

#include "ciphersuites.hpp"
#include "utils.hpp"
#include "log.hpp"

bool ServerHelloParser::parse() {
    try {
        kaitai::kstream ks( data );
        parsed = std::make_unique<tls_server_hello_t>( &ks );
    } catch( std::ios_base::failure ex ) {
        LOG( "Failed to parse : " << ex.what() );
    } catch( std::runtime_error ex ) {
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
    LOG( "type          : " << utils::hex( parsed->type() ) );
    LOG( "size          : " << static_cast<int>( parsed->size() ) );
    LOG( "major         : " << static_cast<int>( parsed->version()->major() ) );
    LOG( "minor         : " << static_cast<int>( parsed->version()->minor() ) );
    LOG( "random TS     : " << utils::toString( parsed->random()->gmt_unix_time() ) );
    LOG( "random        : " << utils::hex( parsed->random()->random() ) );
    LOG( "session ID    : [" << utils::hex( parsed->session_id()->sid() ) << "]" );

    uint16_t suite = parsed->cipher_suite();
    const auto it = ciphersuite::ciphers.find( suite );

    if( it != ciphersuite::ciphers.cend() ) {
        LOG( "cipher suite  : " << utils::hex( boost::endian::native_to_big( suite ) )
             << " " << it->second );
    } else {
        LOG( "cipher suite  : " << utils::hex( boost::endian::native_to_big( suite ) ) );
    }

    LOG( "compression   : " << utils::hex( parsed->compression_method() ) );

    for( const std::unique_ptr<tls_server_hello_t::extension_t>& extension : *parsed->extensions()->extensions() ) {
        uint16_t type = extension->type();
        LOG( "    extension     : " << utils::hex( boost::endian::native_to_big( type ) ) );
    }

}

tls_server_hello_t* ServerHelloParser::hello() const {
    if( parsed ) {
        return parsed.get();
    }

    return nullptr;
}
