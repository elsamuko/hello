#include "client.hpp"
#include "log.hpp"

void usage() {
    LOG( "usage:" );
    LOG( "serverhello <host> <port>" );
}

int main( int argc, char* argv[] ) {
    if( argc < 2 ) {
        usage();
        return 0;
    }

    std::string host = argv[1];
    std::string port = "443";

    if( argc == 3 ) {
        port = argv[2];
    }

    client::run( host, port );
    return 0;
}
