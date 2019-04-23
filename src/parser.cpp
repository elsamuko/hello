#include "parser.hpp"
#include "log.hpp"

#include <iomanip>
#include <chrono>

#include <boost/endian/conversion.hpp>

#include "ciphersuites.hpp"

namespace  {
std::string hex( const size_t size, const void* data ) {
    std::stringstream out;
    const uint8_t* casted = reinterpret_cast<const uint8_t*>( data );

    for( size_t i = 0; i < size; ) {
        out << std::setw( 2 )
            << std::setfill( '0' )
            << std::hex
            << static_cast<int>( casted[i] )
            << " ";

        if( ++i % 16 == 0 ) {
            out << std::endl;
        }
    }

    return out.str();
}

std::string hex( const std::string& data ) {
    return hex( data.size(), data.data() );
}

template<class T>
std::string hex( const T& t ) {
    return hex( sizeof( T ), &t );
}

}

bool Parser::parse() {
    try {
        kaitai::kstream ks( data );
        parsed = std::make_unique<tls_client_hello_t>( &ks );
    } catch( std::ios_base::failure ex ) {
        LOG( "Failed to parse : " << ex.what() );
    }

    return !!parsed;
}

std::string toString( const std::time_t& t ) {
    std::tm* tm = std::gmtime( &t );
    std::stringstream stream;
    stream << std::put_time( tm, "%F %T" );
    return stream.str();
}

void Parser::dump() {
    if( !parsed ) {
        LOG( "parsed is null" );
        return;
    }

    // dump
    LOG( "type          : " << static_cast<int>( parsed->type() ) );
    LOG( "size          : " << static_cast<int>( parsed->size() ) );
    LOG( "major         : " << static_cast<int>( parsed->version()->major() ) );
    LOG( "minor         : " << static_cast<int>( parsed->version()->minor() ) );
    LOG( "random TS     : " << toString( parsed->random()->gmt_unix_time() ) );
    LOG( "random        : " << hex( parsed->random()->random() ) );
    LOG( "session ID    : [" << hex( parsed->session_id()->sid() ) << "]" );

    for( const uint16_t& suite : *parsed->cipher_suites()->cipher_suites() ) {
        const auto it = ciphersuite::ciphers.find( suite );

        if( it != ciphersuite::ciphers.cend() ) {
            LOG( "    suite         : " << hex( boost::endian::native_to_big( suite ) )
                 << " " << it->second );
        } else {
            LOG( "    suite         : " << hex( boost::endian::native_to_big( suite ) ) );
        }
    }

    LOG( "compression   : " << hex( parsed->compression_methods()->compression_methods() ) );

    for( const std::unique_ptr<tls_client_hello_t::extension_t>& extension : *parsed->extensions()->extensions() ) {
        uint16_t type = extension->type();
        LOG( "    extension     : " << hex( boost::endian::native_to_big( type ) ) );

        // SNI
        if( extension->type() == 0 ) {
            tls_client_hello_t::sni_t* sni = reinterpret_cast<tls_client_hello_t::sni_t*>( extension->body() );

            for( const std::unique_ptr<tls_client_hello_t::server_name_t>& name : *sni->server_names() ) {
                LOG( "        server name   : " << name->host_name() );
            }
        }

        // ALPN
        if( extension->type() == 16 ) {
            tls_client_hello_t::alpn_t* alpn = reinterpret_cast<tls_client_hello_t::alpn_t*>( extension->body() );

            for( const std::unique_ptr<tls_client_hello_t::protocol_t>& protocol : *alpn->alpn_protocols() ) {
                LOG( "        alpn protocol  : " << protocol->name() );
            }
        }

    }
}

tls_client_hello_t* Parser::hello() const {
    if( parsed ) {
        return parsed.get();
    }

    return nullptr;
}
