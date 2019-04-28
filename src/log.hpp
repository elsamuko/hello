#pragma once

#include <boost/log/trivial.hpp>

#define LOG( A ) BOOST_LOG_TRIVIAL(info) << A;
#define LOG_DEBUG( A ) LOG( A )
