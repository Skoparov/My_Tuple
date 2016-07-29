#ifndef COMPARE_WITH_TYPES_H
#define COMPARE_WITH_TYPES_H

#include <type_traits>

namespace details
{

template< typename T, typename U, typename std::enable_if_t< std::is_same< T, U >::value >* = nullptr >
bool compare_with_types( const T& t, const U& u  )
{
    return t == u;
}

template< typename T, typename U, typename std::enable_if_t< !std::is_same< T, U >::value >* = nullptr >
bool compare_with_types( const T& t, const U& u  )
{
    return false;
}

}

#endif
