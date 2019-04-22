#define BOOST_TEST_MODULE testparser
#include <boost/test/included/unit_test.hpp>

#include "parser.hpp"
#include "hello_raw.hpp"

BOOST_AUTO_TEST_CASE( parse_good ) {
    std::string data( reinterpret_cast<const char*>( hello_raw ), sizeof( hello_raw ) );
    Parser parser( data );
    BOOST_REQUIRE( parser.parse() );
    parser.dump();
}

BOOST_AUTO_TEST_CASE( parse_bad ) {
    std::string data( "This is not a TLS hello" );
    Parser parser( data );
    BOOST_CHECK( !parser.parse() );
}
