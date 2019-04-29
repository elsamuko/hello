#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>

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

std::string utils::dump( const std::string& data ) {
    std::stringstream out;
    const uint8_t* casted = reinterpret_cast<const uint8_t*>( data.data() );
    size_t size = data.size();

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
