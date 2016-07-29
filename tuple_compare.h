#ifndef TUPLE_COMPARE_H
#define TUPLE_COMPARE_H

#include "compare_with_types.h"

namespace details
{

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index >= tuple_size >* = nullptr >
bool compare_tuples( const T1& t1, const T2& t2 );

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index <= tuple_size - 1 >* = nullptr >
bool compare_tuples( const T1& t1, const T2& t2 );

}
namespace basic
{

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

// Comparison operators

template< class T, class U >
inline bool operator==( const T& t, const U& u )
{
    if( tuple_size( t ) != tuple_size( u ) )
    {
        return false;
    }

    return details::compare_tuples< 0, tuple_size( t ) >( t, u );
}

template< class T, class U >
inline bool operator!=( const T& t, const U& u )
{
    if( tuple_size( t ) != tuple_size( u ) )
    {
        return true;
    }

    return !details::compare_tuples< 0, tuple_size( t ) >( t, u );
}

template< class T, class U >
inline bool operator==( const T&& t, const U&& u )
{
    if( tuple_size( t ) != tuple_size( u ) )
    {
        return false;
    }

    return details::compare_tuples< 0, tuple_size( t ) >(
                std::forward< T >( t ), std::forward< U >( u ) );
}

template< class T, class U >
inline bool operator!=( const T&& t, const U&& u )
{
    if( tuple_size( t ) != tuple_size( u ) )
    {
        return true;
    }

    return !details::compare_tuples< 0, tuple_size( t ) >(
                std::forward< const T >( t ), std::forward< const U >( u ) );
}

}

namespace details
{

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index >= tuple_size >* = nullptr >
bool compare_tuples( const T1& t1, const T2& t2 )
{
    return true;
}

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index <= tuple_size - 1 >* = nullptr >
bool compare_tuples( const T1& t1, const T2& t2 )
{
    const auto& v1 = basic::get< index >( t1 );
    const auto& v2 = basic::get< index >( t2 );

    if( !compare_with_types< decltype( v1 ), decltype( v2 ) >( v1, v2 ) ){
        return false;
    }

    return compare_tuples< index + 1, tuple_size >( t1, t2 );
}

}


#endif
