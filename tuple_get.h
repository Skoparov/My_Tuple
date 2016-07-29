#ifndef TUPLE_GET_H
#define TUPLE_GET_H

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

// Acquires the type of k-th element of a tuple

template< size_t k, typename... Args >
using elem_type_t = typename details::elem_type< k, Args... >::type;

// Returns a reference to the k-th element of a tuple

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
std::enable_if_t< k != 0, elem_type_t< k, Curr, Tail... >& >
get( T< Curr, Tail... >& t )
{
    T< Tail... >& base = t;
    return get< k-1, T, Tail... >( base );
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
std::enable_if_t< k == 0, Curr& >
get( T< Curr, Tail... >& t )
{
    return t.data;
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
constexpr std::enable_if_t< k != 0, elem_type_t< k, Curr, Tail... >const& >
get( const T< Curr, Tail... >& t )
{     
    return get< k-1, T, Tail... >( ( const T< Tail... >& )t );
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
constexpr std::enable_if_t< k == 0, Curr const& >
get( const T< Curr, Tail... >& t )
{
    return t.data;
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
std::enable_if_t< k != 0, elem_type_t< k, Curr, Tail... >&& >
get( T< Curr, Tail... >&& t )
{    
    return get< k-1, T, Tail... >( std::forward< T< Tail... > >( t ) );
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
std::enable_if_t< k == 0, Curr&& >
get( T< Curr, Tail... >&& t )
{
    return std::move( t.data );
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
constexpr typename std::enable_if< k != 0, elem_type_t< k, Curr, Tail... > const&& >::type
get( const T< Curr, Tail... >&& t )
{
    return get< k-1, T, Tail... >( std::forward< const T< Tail... > >( t ) );
}

template< size_t k, template< typename... > class T, typename Curr, typename... Tail >
constexpr std::enable_if< k == 0, const Curr&& >
get( const T< Curr, Tail... >&& t )
{
    return std::move( t.data );
}

}

namespace details
{

template< size_t k, typename T, typename... Tail >
struct elem_type
{
    using type = typename elem_type< k-1, Tail... >::type;
};

template< typename T, typename... Tail >
struct elem_type< 0, T, Tail... >
{
    using type = T;
};

}

#endif
