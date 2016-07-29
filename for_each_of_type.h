#ifndef FOR_EACH_OF_TYPE_H
#define FOR_EACH_OF_TYPE_H

#include "tuple_get.h"

namespace details
{

template< typename, typename Functor, size_t index,
          template< typename... > class T, typename... Args,
          typename std::enable_if_t< index >= sizeof...( Args ) >* = nullptr >
void for_each_of_type( T< Args... >&, Functor& );

template< typename, typename Functor, size_t index,
          template< typename... > class T, typename... Args,
          typename std::enable_if_t<  index <= sizeof...( Args ) - 1  >* = nullptr >
void for_each_of_type( T< Args... >&, Functor& );

}

namespace basic
{

template< typename Target, typename Functor, typename T >
void for_each_of_type_t( T& t, Functor& f )
{
    constexpr size_t next_index = first_of_type< Target >( t );
    details::for_each_of_type< Target, Functor, next_index >( t, f );
}


}

namespace details
{

template< typename Target, typename Functor, size_t index,
          template< typename... > class T, typename... Args,
          typename std::enable_if_t< index >= sizeof...( Args ) >* = nullptr >
void for_each_of_type( T< Args... >& t, Functor& f )
{

}

template< typename Target, typename Functor, size_t index,
          template< typename... > class T, typename... Args,
          typename std::enable_if_t<  index <= sizeof...( Args ) - 1  >* = nullptr >
void for_each_of_type( T< Args... >& t, Functor& f )
{
    f( basic::get< index >( t ) );

    constexpr size_t next_index = first_of_type< Target, index + 1 >( t );
    for_each_of_type< Target, Functor, next_index >( t, f );
}

}

#endif
