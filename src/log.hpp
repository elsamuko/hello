#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <ostream>

class Logger {
    public:
        Logger( const char* file, int line, const char* function ) {
            m.lock();
            timestamp();
            out.fill( ' ' );
            out.width( 25 );
            out << ( strrchr( file, '/' ) + 1 ) << " ";
            out.width( 25 );
            out << function << "(";
            out.width( 4 );
            out.setf( std::ios::right );
            out << line << "): ";
            intendation = out.tellp();
        }
        ~Logger() {
            std::string line;
            std::string whitespaces( intendation, ' ' );

            if( std::getline( out, line ) ) {
                std::cout << line << std::endl;

                while( std::getline( out, line ) ) {
                    std::cout << whitespaces << line << std::endl;
                }
            }

            m.unlock();
        }
        template <class T>
        std::ostream& operator<<( const T& t ) {
            out << t;
            return out;
        }
        void timestamp() {

            auto tp = std::chrono::system_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( tp.time_since_epoch() );
            size_t modulo = ms.count() % 1000;

            time_t seconds = std::chrono::duration_cast<std::chrono::seconds>( ms ).count();

            out << std::put_time( std::gmtime( &seconds ), "%T." );

            // ms
            out.fill( '0' );
            out.width( 3 );
            out << modulo;
        }
    private:
        size_t intendation = 0;
        std::stringstream out;
        std::mutex m;
};

#define LOG( A ) Logger(__FILE__, __LINE__, __FUNCTION__) << A;
