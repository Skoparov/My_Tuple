#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include <type_traits>

namespace details
{

template< size_t k, typename T, typename... Tail >
struct elem_type;

template< typename T, typename... Tail >
struct elem_type< 0, T, Tail... >;

}

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

// Creates a tuple from arbitary number of arguments

template< typename... Args >
my_tuple< Args... > make_my_tuple( const Args&... args )
{
    return my_tuple< Args... >( args... );
}

template< typename... Args >
my_tuple< Args... > make_my_tuple( Args&&... args )
{
    return my_tuple< Args... >( std::forward< Args >( args )... );
}

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

// Acquires the type of k-th element of a tuple

template< size_t k, typename... Args >
using elem_type_t = typename details::elem_type< k, Args... >::type;

// Returns a reference to the k-th element of a tuple

template< size_t k, typename T, typename... Tail >
std::enable_if_t< k != 0, elem_type_t< k, T, Tail... >& >
get( my_tuple< T, Tail... >& t )
{
    my_tuple< Tail... >& base = t;
    return get< k-1, Tail... >( base );
}

template< size_t k, typename T, typename... Tail >
std::enable_if_t< k == 0, T& >
get( my_tuple< T, Tail... >& t )
{
    return t.data;
}

template< size_t k, typename T, typename... Tail >
constexpr std::enable_if_t< k != 0, elem_type_t< k, T, Tail... >const& >
get( const my_tuple< T, Tail... >& t )
{
    my_tuple< Tail... >& base = t;
    return get< k-1, Tail... >( base );
}

template< size_t k, typename T, typename... Tail >
constexpr std::enable_if_t< k == 0, T const& >
get( const my_tuple< T, Tail... >& t )
{
    return t.data;
}

template< size_t k, typename T, typename... Tail >
std::enable_if_t< k != 0, elem_type_t< k, T, Tail... >&& >
get( my_tuple< T, Tail... >&& t )
{
    my_tuple< Tail... >& base = t;
    return get< k-1, Tail... >( base );
}

template< size_t k, typename T, typename... Tail >
std::enable_if_t< k == 0, T&& >
get( my_tuple< T, Tail... >&& t )
{
    return std::move( t.data );
}

template< size_t k, typename T, typename... Tail >
constexpr typename std::enable_if< k != 0, elem_type_t< k, T, Tail... > const&& >::type
get( const my_tuple< T, Tail... >&& t )
{
    my_tuple< Tail... >& base = t;
    return get< k-1, Tail... >( base );
}

template< size_t k, typename T, typename... Tail >
constexpr std::enable_if< k == 0, const T&& >
get( const my_tuple< T, Tail... >&& t )
{
    return std::move( t.data );
}

}


#endif
