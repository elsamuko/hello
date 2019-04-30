#include "client.hpp"
#include "log.hpp"

void usage() {
    LOG( "usage:" );
    LOG( "serverhello <host> <port>" );
}

int main( int argc, char* argv[] ) {
    if( argc != 3 ) {
        usage();
        return 0;
    }

    client::run( argv[1], argv[2] );
    return 0;
}
