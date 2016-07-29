#ifndef TUPLE_COPY_H
#define TUPLE_COPY_H

#include "tuple_get.h"

namespace details
{

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( const T< Args1... >& source, T< Args2... >& target );

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( const T< Args1... >&, T< Args2... >& );

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( T< Args1... >&& source, T< Args2... >& target );

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( T< Args1... >&&, T< Args2... >& );

}

namespace basic
{

// Copies the elements of one tuple to another

template< size_t begin = 0, size_t destBegin = 0, size_t count = 0,
          template< typename... > class T, typename... Args1, typename... Args2 >
void copy_tuple( const T< Args1... >& source, T< Args2... >& target )
{    
    static_assert( count >= 0, "Number of objects to be copied must be >= 0" );

    constexpr size_t actualCount = count > 0? count : sizeof...( Args1 );
    static_assert( actualCount <= sizeof...( Args2 ), "Number of objects to be copied must be <= dest dize" );

    details::copy_tuple< begin, destBegin, actualCount >( source, target );
}

template< size_t begin = 0, size_t destBegin = 0, size_t count = 0,
          template< typename... > class T, typename... Args1, typename... Args2 >
void copy_tuple( T< Args1... >&& source, T< Args2... >& target )
{
    static_assert( count >= 0, "Number of objects to be copied must be >= 0" );

    constexpr size_t actualCount = count > 0? count : sizeof...( Args1 );
    static_assert( actualCount <= sizeof...( Args2 ), "Number of objects to be copied must be <= dest dize" );

    details::copy_tuple< begin, destBegin, actualCount >( std::forward< T< Args1... > >( source ), target );
}

}

namespace details
{

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( const T< Args1... >& source, T< Args2... >& target )
{
    basic::get< destIndex >(target) = basic::get< index >( source );
    return details::copy_tuple< index + 1, destIndex + 1, count - 1 >( source, target );
}

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( const T< Args1... >&, T< Args2... >& )
{

}

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( T< Args1... >&& source, T< Args2... >& target )
{
    basic::get< destIndex >(target) = std::move( basic::get< index >( source ) );
    return details::copy_tuple< index + 1, destIndex + 1, count - 1 >( std::forward< T< Args1... > >( source ), target );
}

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( T< Args1... >&&, T< Args2... >& )
{

}

}

#endif
