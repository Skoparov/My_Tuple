#ifndef TUPLE_FIND_H
#define TUPLE_FIND_H

#include "tuple_get.h"

namespace details
{

template< typename Target, size_t index, size_t startIndex, size_t tuple_size, typename T,
          typename std::enable_if_t< index >= tuple_size >* = nullptr >
constexpr size_t tuple_find( Target&&, T&&  )
{
    return tuple_size;
}

template< typename Target, size_t index, size_t startIndex, size_t tuple_size, typename T,
          typename std::enable_if_t< index <= tuple_size -1 >* = nullptr >
constexpr size_t tuple_find( Target&& target, T&& t  )
{
    return basic::get< index >( t ) == target && index >= startIndex?
                index :
                tuple_find< Target,
                            first_of_type< Target, index + 1 >( t ),
                            startIndex,
                            tuple_size >
                            ( std::forward< Target >( target ), std::forward< T >( t ) );
}

}

namespace basic
{

template< typename Target, size_t startIndex = 0, typename T >
constexpr size_t tuple_find( Target&& target, T&& t )
{
    return details::tuple_find< Target,
                                first_of_type< Target >( t ),
                                startIndex,
                                tuple_size( t ) >
                                ( std::forward< Target >( target ), std::forward< T >( t ) );
}

}

#endif
