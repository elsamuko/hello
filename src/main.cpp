#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/endian/conversion.hpp>

#include <string>
#include <iostream>
#include <iomanip>

#include "kaitai/tls_client_hello.h"
#include "ciphersuites.hpp"

#define LOG( A ) std::cout << A << std::endl;
#define LOG_DEBUG( A ) LOG( A )

namespace {
boost::asio::io_service ioservice;
boost::asio::ip::tcp::acceptor tcp_acceptor{ioservice, {boost::asio::ip::tcp::v4(), 2014}};
boost::asio::ip::tcp::socket tcp_socket{ioservice};
std::string dataTls;
std::string dataHello;
}

uint32_t toLittleEndianfrom3Bytes( uint32_t size ) {
    uint8_t* arr = reinterpret_cast<uint8_t*>( &size );
    uint32_t result = ( static_cast<uint32_t>( arr[0] ) << 16 ) |
                      ( static_cast<uint32_t>( arr[1] ) << 8 ) |
                      ( static_cast<uint32_t>( arr[2] ) );
    return result;
}

std::string dump( const size_t size, const void* data ) {
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

std::string dump( const std::string& data ) {
    return dump( data.size(), data.data() );
}

template<class T>
std::string dump( const T& t ) {
    return dump( sizeof( T ), &t );
}

void dumpHello( std::string& hello ) {

    // pass to kaitai generated parser
    kaitai::kstream ks( hello );
    tls_client_hello_t parsed( &ks );

    // dump
    LOG( "major         : " << static_cast<int>( parsed.version()->major() ) );
    LOG( "minor         : " << static_cast<int>( parsed.version()->minor() ) );
    LOG( "random TS     : " << static_cast<size_t>( parsed.random()->gmt_unix_time() ) );
    LOG( "random        : " << dump( parsed.random()->random() ) );
    LOG( "session ID    : [" << dump( parsed.session_id()->sid() ) << "]" );

    for( const uint16_t& suite : *parsed.cipher_suites()->cipher_suites() ) {
        const auto it = ciphersuite::ciphers.find( suite );

        if( it != ciphersuite::ciphers.cend() ) {
            LOG( "    suite         : " << dump( boost::endian::native_to_big( suite ) )
                 << " " << it->second );
        } else {
            LOG( "    suite         : " << dump( boost::endian::native_to_big( suite ) ) );
        }
    }

    LOG( "compression   : " << dump( parsed.compression_methods()->compression_methods() ) );

    for( const std::unique_ptr<tls_client_hello_t::extension_t>& extension : *parsed.extensions()->extensions() ) {
        uint16_t type = extension->type();
        LOG( "    extension     : " << dump( boost::endian::native_to_big( type ) ) );

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

void read_ClientHello( const boost::system::error_code& /*ec*/,
                       std::size_t bytes_transferred ) {

    LOG( bytes_transferred << "B" );
    // LOG( dump( bytes_transferred, reinterpret_cast<const uint8_t*>( dataHello.data() ) ) );

    std::string withOffset = dataHello.substr( 4 );
    dumpHello( withOffset );

    LOG_DEBUG( "\nShutting down connection" );
    tcp_socket.shutdown( boost::asio::ip::tcp::socket::shutdown_send );
}

// https://wiki.osdev.org/TLS_Handshake
typedef struct __attribute__( ( packed ) ) {
    uint8_t content_type;  // 0x16
    uint16_t version;
    uint16_t length;
} TLSRecord;

void read_TLSRecord( const boost::system::error_code& /*ec*/,
                     std::size_t bytes_transferred ) {

    LOG( bytes_transferred << "B" );
    LOG( dump( dataTls ) );

    const TLSRecord* record = reinterpret_cast<const TLSRecord*>( dataTls.data() );

    LOG( "\n" );
    LOG( "Type          : " << std::hex << static_cast<int>( record->content_type ) );
    LOG( "TLS Version   : " << std::hex << boost::endian::big_to_native( record->version ) );
    LOG( "Length        : " << std::dec << boost::endian::big_to_native( record->length ) );

    dataHello.resize( boost::endian::big_to_native( record->length ) );
    boost::asio::async_read( tcp_socket, boost::asio::buffer( &dataHello[0], dataHello.size() ), read_ClientHello );
}

void accept_handler( const boost::system::error_code& ec ) {
    if( !ec ) {
        dataTls.resize( sizeof( TLSRecord ) );
        boost::asio::async_read( tcp_socket, boost::asio::buffer( &dataTls[0], dataTls.size() ), read_TLSRecord );
    }
}

// trigger with
// openssl s_client -servername "localhost" -connect localhost:2014
int main() {
    tcp_acceptor.listen();
    tcp_acceptor.async_accept( tcp_socket, accept_handler );
    ioservice.run();
}
