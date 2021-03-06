#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include "details/tuple_concat.h"
#include "details/tuple_compare.h"
#include "details/for_each_of_type.h"
#include "details/tuple_find.h"

namespace basic
{

// Basic tuple implementation
template< typename... Args >
struct my_tuple
{
    using tuple_tag = void;
};

template< typename T, typename... Tail >
struct my_tuple< T, Tail... > : my_tuple< Tail... >
{
    constexpr my_tuple( const T& d, const Tail&... tail ) :
        my_tuple< Tail... >( tail... ),
        data( d ){}

    constexpr my_tuple( T&& d, Tail&&... tail ) :
        my_tuple< Tail... >( std::forward< Tail >( tail )... ),
        data( std::move( d ) ){}

    constexpr my_tuple() :
        my_tuple< Tail... >(),
        data( T() ){}

    T data;
};

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
