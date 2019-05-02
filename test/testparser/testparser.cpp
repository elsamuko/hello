#define BOOST_TEST_MODULE testparser
#include <boost/test/included/unit_test.hpp>

#include "clienthelloparser.hpp"
#include "serverhelloparser.hpp"
#include "client_hello_raw.hpp"
#include "server_hello_news.ycombinator.com.hpp"
#include "server_hello_heise.de.hpp"

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

void testParseGood( const unsigned char cdata[], const size_t size, const uint64_t tlsSize ) {
    std::string data( reinterpret_cast<const char*>( cdata ), size );
    ServerHelloParser parser( data );
    BOOST_REQUIRE( parser.parse() );
    parser.dump();
    BOOST_CHECK_EQUAL( parser.hello()->size(), tlsSize );
}

BOOST_AUTO_TEST_CASE( server_parse_good ) {
    testParseGood( server_hello_news_ycombinator_com_raw, sizeof( server_hello_news_ycombinator_com_raw ), 57 );
    testParseGood( server_hello_heise_de_raw, sizeof( server_hello_heise_de_raw ), 61 );
}

BOOST_AUTO_TEST_CASE( server_parse_bad ) {
    std::string data( "This is not a TLS hello" );
    ServerHelloParser parser( data );
    BOOST_CHECK( !parser.parse() );
}
