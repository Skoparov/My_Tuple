#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include "tuple_concat.h"

namespace basic
{

// Basic tuple implementation

template< typename... Args >
struct my_tuple{};

template< typename T, typename... Tail >
struct my_tuple< T, Tail... > : my_tuple< Tail... >
{
    my_tuple( const T& d, const Tail&... tail ) : my_tuple< Tail... >( tail... ), data( d ){}
    my_tuple( T&& d, Tail&&... tail ) : my_tuple< Tail... >( std::forward< Tail >( tail )... ), data( std::move( d ) ){}
    my_tuple() : my_tuple< Tail... >() {}

    T data;
};

// Acquires tuple size

template< template< typename... > class T, typename...Args >
constexpr size_t tuple_size( const T< Args... >& t )
{
    return sizeof...(Args);
}

template< template< typename... > class T, typename...Args >
constexpr size_t tuple_size( const T< Args... >&& t )
{
    return sizeof...(Args);
}

// Creates a tuple from arbitary number of arguments

template< typename... Args >
constexpr my_tuple< Args... > make_my_tuple( const Args&... args )
{
    return my_tuple< Args... >( args... );
}

template< typename... Args >
constexpr my_tuple< Args... > make_my_tuple( Args&&... args )
{
    return my_tuple< Args... >( std::forward< Args >( args )... );
}

}

#endif
