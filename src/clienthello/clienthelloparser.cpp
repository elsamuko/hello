#include "clienthelloparser.hpp"

#include <boost/endian/conversion.hpp>

#include "ciphersuites.hpp"
#include "extensions.hpp"
#include "utils.hpp"
#include "log.hpp"

bool ClientHelloParser::parse() {
    try {
        kaitai::kstream ks( data );
        parsed = std::make_unique<tls_client_hello_t>( &ks );
    } catch( std::ios_base::failure ex ) {
        LOG( "Failed to parse : " << ex.what() );
    } catch( std::runtime_error ex ) {
        LOG( "Failed to parse : " << ex.what() );
    }

    return !!parsed;
}

void ClientHelloParser::dump() {
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

    // openssl ciphers -V
    for( const uint16_t& suite : *parsed->cipher_suites()->cipher_suites() ) {
        const auto it = ciphersuite::ciphers.find( suite );

        if( it != ciphersuite::ciphers.cend() ) {
            LOG( "    suite         : " << utils::hex( boost::endian::native_to_big( suite ) )
                 << " " << it->second );
        } else {
            LOG( "    suite         : " << utils::hex( boost::endian::native_to_big( suite ) ) );
        }
    }

    LOG( "compression   : " << utils::hex( parsed->compression_methods()->compression_methods() ) );

    // https://www.iana.org/assignments/tls-extensiontype-values/tls-extensiontype-values.xhtml
    for( const std::unique_ptr<tls_client_hello_t::extension_t>& extension : *parsed->extensions()->extensions() ) {
        uint16_t type = extension->type();
        const auto it = extension::extensions.find( type );

        if( it != ciphersuite::ciphers.cend() ) {
            LOG( "    extension     : " << utils::hex( boost::endian::native_to_big( type ) )
                 << " " << it->second );
        } else {
            LOG( "    extension     : " << utils::hex( boost::endian::native_to_big( type ) ) );
        }

        // SNI
        if( extension->type() == 0 ) {
            tls_client_hello_t::sni_t* sni = reinterpret_cast<tls_client_hello_t::sni_t*>( extension->body() );

            for( const std::unique_ptr<tls_client_hello_t::server_name_t>& name : *sni->server_names() ) {
                LOG( "        server_name   : " << name->host_name() );
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

tls_client_hello_t* ClientHelloParser::hello() const {
    if( parsed ) {
        return parsed.get();
    }

    return nullptr;
}
