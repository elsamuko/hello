#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

void utils::toFile( const std::string& filename, const std::string& data ) {
    std::ofstream file( filename.c_str(), std::ios::out | std::ios::binary );

    if( file.is_open() ) {
        file.write( data.data(), data.size() );
    }
}

std::string utils::hex( const size_t size, const void* data ) {
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

std::string utils::hex( const std::string& data ) {
    return hex( data.size(), data.data() );
}

std::string utils::toString( const std::time_t& t ) {
    std::tm* tm = std::gmtime( &t );
    std::stringstream stream;
    stream << std::put_time( tm, "%F %T" );
    return stream.str();
}
