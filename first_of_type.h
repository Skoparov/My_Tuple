#ifndef FIRST_OF_TYPE_H
#define FIRST_OF_TYPE_H

#include "args_size.h"

namespace details
{

template< typename, size_t, size_t, typename, typename... Tail,
          typename std::enable_if_t< args_size< Tail... >() == 0 >* = nullptr >
constexpr size_t first_of_type();

template< typename Target, size_t, size_t, typename, typename... Tail,
          typename std::enable_if_t< args_size< Tail... >() != 0 >* = nullptr >
constexpr size_t first_of_type();

}

namespace basic
{

template< typename Target, size_t startIndex = 0, template< typename... > class T, typename... Args >
constexpr size_t first_of_type( const T< Args... >& t )
{
    static_assert( startIndex <= sizeof...(Args) + 1, "Start index in invalid" );
    return details::first_of_type< Target, startIndex, 0, Args... >();
}

template< typename Target, size_t startIndex = 0, template< typename... > class T, typename... Args >
constexpr size_t first_of_type( const T< Args... >&& t )
{
    static_assert( startIndex <= sizeof...(Args) + 1, "Start index in invalid" );
    return details::first_of_type< Target, startIndex, 0, Args... >();
}

}

namespace details
{

template< typename Target,
          size_t startIndex = 0,
          size_t index,
          typename T, typename... Tail,
          typename std::enable_if_t< args_size< Tail... >() == 0 >* = nullptr >
constexpr size_t first_of_type()
{
    return std::is_same< T, Target >::value && startIndex >= index?
                index : index + 1;
}

template< typename Target,
          size_t startIndex = 0,
          size_t index = 0,
          typename T, typename... Tail,
          typename std::enable_if_t< args_size< Tail... >() != 0 >* = nullptr >
constexpr size_t first_of_type()
{
    return std::is_same< T, Target >::value &&  startIndex >= index ?
                index : details::first_of_type< Target, startIndex, index + 1, Tail... >();
}

}

#endif
