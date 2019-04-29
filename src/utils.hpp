#pragma once

#include <string>

namespace utils {
void toFile( const std::string& filename, const std::string& data );

std::string hex( const size_t size, const void* data );
std::string hex( const std::string& data );

template<class T>
std::string hex( const T& t ) {
    return hex( sizeof( T ), &t );
}

std::string dump( const std::string& data );

}

