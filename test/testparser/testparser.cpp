#define BOOST_TEST_MODULE testparser
#include <boost/test/included/unit_test.hpp>

#include "clienthelloparser.hpp"
#include "client_hello_raw.hpp"

BOOST_AUTO_TEST_CASE( parse_good ) {
    std::string data( reinterpret_cast<const char*>( client_hello_raw ), sizeof( client_hello_raw ) );
    ClientHelloParser parser( data );
    BOOST_REQUIRE( parser.parse() );
    parser.dump();
    BOOST_CHECK_EQUAL( parser.hello()->size(), 327 );
}

BOOST_AUTO_TEST_CASE( parse_bad ) {
    std::string data( "This is not a TLS hello" );
    ClientHelloParser parser( data );
    BOOST_CHECK( !parser.parse() );
}
