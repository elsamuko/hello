#include "server.hpp"

#include <iomanip>

#include <boost/endian/conversion.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "parser.hpp"
#include "log.hpp"

namespace {
boost::asio::io_service ioservice;
boost::asio::ip::tcp::acceptor tcp_acceptor{ioservice, {boost::asio::ip::tcp::v4(), 2014}};
boost::asio::ip::tcp::socket tcp_socket{ioservice};
std::string dataTls;
std::string dataHello;
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

void read_ClientHello( const boost::system::error_code& /*ec*/,
                       std::size_t bytes_transferred ) {

    LOG( bytes_transferred << "B" );
    // LOG( dump( bytes_transferred, reinterpret_cast<const uint8_t*>( dataHello.data() ) ) );

    Parser parser( dataHello );
    parser.parse();
    parser.dump();

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

void server::run() {
    tcp_acceptor.listen();
    tcp_acceptor.async_accept( tcp_socket, accept_handler );
    ioservice.run();
}
