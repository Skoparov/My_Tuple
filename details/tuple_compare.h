#ifndef TUPLE_COMPARE_H
#define TUPLE_COMPARE_H

#include "compare_with_types.h"

namespace details
{

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index >= tuple_size >* = nullptr >
constexpr bool compare_tuples( const T1& t1, const T2& t2 );

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index <= tuple_size - 1 >* = nullptr >
constexpr bool compare_tuples( const T1& t1, const T2& t2 );

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
constexpr inline bool operator==( const T& t, const U& u )
{
    return tuple_size( t ) != tuple_size( u )?
                false :
                details::compare_tuples< 0, tuple_size( t ) >( t, u );
}

template< class T, class U >
constexpr inline bool operator!=( const T& t, const U& u )
{
    return tuple_size( t ) == tuple_size( u )?
                !details::compare_tuples< 0, tuple_size( t ) >( t, u ) :
                true;
}

template< class T, class U >
constexpr inline bool operator==( const T&& t, const U&& u )
{
    return tuple_size( t ) != tuple_size( u )?
                false :
                details::compare_tuples< 0, tuple_size( t ) >(
                    std::forward< const T >( t ), std::forward< const U >( u ) );
}

template< class T, class U >
constexpr inline bool operator!=( const T&& t, const U&& u )
{
    return tuple_size( t ) == tuple_size( u )?
                !details::compare_tuples< 0, tuple_size( t ) >(
                    std::forward< const T >( t ), std::forward< const U >( u ) ) :
                true;
}

}

namespace details
{

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index >= tuple_size >* = nullptr >
constexpr bool compare_tuples( const T1& t1, const T2& t2 )
{
    return true;
}

template< size_t index, size_t tuple_size, typename T1, typename T2,
          std::enable_if_t< index <= tuple_size - 1 >* = nullptr >
constexpr bool compare_tuples( const T1& t1, const T2& t2 )
{
    using namespace basic;
    using value_type1 = decltype( get< index >( t1 ) );
    using value_type2 = decltype( get< index >( t2 ) );

    return compare_with_types< value_type1, value_type2 > ( get< index >( t1 ), get< index >( t2 ) ) == false?
                false :
                compare_tuples< index + 1, tuple_size >( t1, t2 );
}

}


#endif
