#include "client.hpp"

#include <boost/endian/conversion.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "serverhelloparser.hpp"
#include "kaitai/tls_record.h"
#include "log.hpp"
#include "utils.hpp"
#include "client_hello_raw.hpp"

// derived from
// https://theboostcpplibraries.com/boost.asio-network-programming#ex.asio_06
class Client {
    private:
        boost::asio::ip::tcp::socket tcp_socket;
        std::string dataClientTls;
        std::string dataClientHello;
        std::string dataServerTls;
        std::string dataServerHello;

    private:
        void read_ServerHello( const boost::system::error_code& /*ec*/,
                               std::size_t bytes_transferred ) {

            LOG( bytes_transferred << "B" );
            utils::toFile( "server_hello.raw", dataServerHello );
            LOG( utils::dump( dataServerHello ) );

            ServerHelloParser parser( dataServerHello );
            parser.parse();
            parser.dump();

            LOG_DEBUG( "\nFinished" );
            tcp_socket.close();
        }
        void read_TLSRecord( const boost::system::error_code& /*ec*/,
                             std::size_t bytes_transferred ) {

            LOG( bytes_transferred << "B" );
            LOG( utils::dump( dataServerTls ) );

            kaitai::kstream ks( dataServerTls );
            tls_record_t kt_record( &ks );

            LOG( "\n" );
            LOG( "Type          : " << std::hex << static_cast<int>( kt_record.type() ) );
            LOG( "TLS Version   : " << std::hex << kt_record.version() );
            LOG( "Length        : " << std::dec << kt_record.length() );

            dataServerHello.resize( kt_record.length() );
            boost::asio::async_read( tcp_socket,
                                     boost::asio::buffer( dataServerHello ),
                                     boost::bind( &Client::read_ServerHello, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) );
        }

        typedef struct __attribute__( ( packed ) ) {
            uint8_t content_type;  // 0x16
            uint16_t version;
            uint16_t length;
        } TLSRecord;

        void write_ClientHello() {
            dataClientTls = { 0x16, 0x03, 0x01, 0x01, 0x4b };
            size_t written = boost::asio::write( tcp_socket, boost::asio::buffer( dataClientTls ) );

            dataClientHello = std::string( reinterpret_cast<const char*>( client_hello_raw ), sizeof( client_hello_raw ) );
            size_t written2 = boost::asio::write( tcp_socket, boost::asio::buffer( dataClientHello ) );

            size_t TLSRecordSize = 5;
            dataServerTls.resize( TLSRecordSize );

            boost::asio::async_read( tcp_socket,
                                     boost::asio::buffer( dataServerTls ),
                                     boost::bind( &Client::read_TLSRecord, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) );
        }

    public:
        Client( boost::asio::io_context& iocontext, const std::string& host, const std::string& port ) :
            tcp_socket{iocontext} {

            boost::asio::ip::tcp::resolver resolver( iocontext );
            boost::asio::ip::tcp::resolver::results_type results = resolver.resolve( host, port );

            boost::asio::connect( tcp_socket, results );
            write_ClientHello();
        }
};

void client::run() {
    boost::asio::io_context iocontext;
    Client client( iocontext, "news.ycombinator.com", "443" );
    iocontext.run();
}
