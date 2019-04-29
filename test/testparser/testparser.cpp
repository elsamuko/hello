#define BOOST_TEST_MODULE testparser
#include <boost/test/included/unit_test.hpp>

#include "clienthelloparser.hpp"
#include "serverhelloparser.hpp"
#include "client_hello_raw.hpp"
#include "server_hello_raw.hpp"

BOOST_AUTO_TEST_CASE( client_parse_good ) {
    std::string data( reinterpret_cast<const char*>( client_hello_raw ), sizeof( client_hello_raw ) );
    ClientHelloParser parser( data );
    BOOST_REQUIRE( parser.parse() );
    parser.dump();
    BOOST_CHECK_EQUAL( parser.hello()->size(), 327 );
}

BOOST_AUTO_TEST_CASE( client_parse_bad ) {
    std::string data( "This is not a TLS hello" );
    ClientHelloParser parser( data );
    BOOST_CHECK( !parser.parse() );
}

BOOST_AUTO_TEST_CASE( server_parse_good ) {
    std::string data( reinterpret_cast<const char*>( server_hello_raw ), sizeof( server_hello_raw ) );
    ServerHelloParser parser( data );
    BOOST_REQUIRE( parser.parse() );
    parser.dump();
    BOOST_CHECK_EQUAL( parser.hello()->size(), 61 );
}

BOOST_AUTO_TEST_CASE( server_parse_bad ) {
    std::string data( "This is not a TLS hello" );
    ServerHelloParser parser( data );
    BOOST_CHECK( !parser.parse() );
}
