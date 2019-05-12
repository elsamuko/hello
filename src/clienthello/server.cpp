#include "server.hpp"

#include <boost/endian/conversion.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "clienthelloparser.hpp"
#include "kaitai/tls_record.h"
#include "hellogenerator.hpp"
#include "utils.hpp"
#include "log.hpp"

// derived from
// https://theboostcpplibraries.com/boost.asio-network-programming#ex.asio_06
class Server {
    private:
        boost::asio::ip::tcp::socket tcp_socket;
        boost::asio::ip::tcp::endpoint tcp_client_endpoint;
        boost::asio::ip::tcp::endpoint tcp_server_endpoint;
        boost::asio::ip::tcp::acceptor tcp_acceptor;
        std::string dataClientTls;
        std::string dataClientHello;
        std::string dataServerTls;
        std::string dataServerHello;

    private:
        void read_ClientHello( const boost::system::error_code& /*ec*/,
                               std::size_t bytes_transferred ) {

            LOG( bytes_transferred << "B" );
            LOG( utils::hex( dataClientHello ) );

            ClientHelloParser parser( dataClientHello );
            parser.parse();
            parser.dump();

            dataServerHello = hello::server();
            TLSRecord record;
            record.length = boost::endian::native_to_big( static_cast<uint16_t>( dataServerHello.size() ) );

            dataServerTls = std::string( reinterpret_cast<const char*>( &record ), sizeof( record ) );
            size_t written = boost::asio::write( tcp_socket, boost::asio::buffer( dataServerTls ) );
            assert( written == dataServerTls.size() );

            LOG( "Sending server hello" );
            size_t written2 = boost::asio::write( tcp_socket, boost::asio::buffer( dataServerHello ) );
            assert( written2 == dataServerHello.size() );

            LOG( "Finished" );
            tcp_socket.close();
        }
        void read_TLSRecord( const boost::system::error_code& /*ec*/,
                             std::size_t bytes_transferred ) {

            LOG( bytes_transferred << "B" );
            LOG( utils::hex( dataClientTls ) );

            kaitai::kstream ks( dataClientTls );
            tls_record_t kt_record( &ks );

            LOG( "\n" );
            LOG( "Type          : " << std::hex << static_cast<int>( kt_record.type() ) );
            LOG( "TLS Version   : " << std::hex << kt_record.version() );
            LOG( "Length        : " << std::dec << kt_record.length() );

            dataClientHello.resize( kt_record.length() );
            boost::asio::async_read( tcp_socket,
                                     boost::asio::buffer( dataClientHello ),
                                     boost::bind( &Server::read_ClientHello, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) );
        }
        void accept_handler( const boost::system::error_code& /*ec*/ ) {
            size_t TLSRecordSize = 5;
            dataClientTls.resize( TLSRecordSize );
            LOG( tcp_server_endpoint << " << " << tcp_client_endpoint );
            LOG( tcp_socket.available() );
            boost::asio::async_read( tcp_socket,
                                     boost::asio::buffer( dataClientTls ),
                                     boost::bind( &Server::read_TLSRecord, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) );
        }

    public:
        Server( boost::asio::io_context& iocontext ) :
            tcp_socket{iocontext},
            tcp_server_endpoint{boost::asio::ip::tcp::v4(), 2014},
            tcp_acceptor{iocontext, tcp_server_endpoint} {

            LOG( "Visit https://" << tcp_server_endpoint );
            LOG( "openssl s_client -servername \"localhost\" -connect " << tcp_server_endpoint );

            tcp_acceptor.listen();
            tcp_acceptor.async_accept( tcp_socket,
                                       tcp_client_endpoint,
                                       boost::bind( &Server::accept_handler, this, boost::asio::placeholders::error ) );
        }
};

void server::run() {
    boost::asio::io_context iocontext;
    Server server( iocontext );
    iocontext.run();
}
