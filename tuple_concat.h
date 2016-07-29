#ifndef TUPLE_CONCAT_H
#define TUPLE_CONCAT_H

#include "tuple_copy.h"

namespace details
{

template< typename... Tail >
struct concat_type;

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2, typename... TailArgs >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... >, TailArgs... >;

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2 >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... > >;

}

namespace basic
{

// Acquires the type of several tuples' concatenation result

template< typename... Args >
using concat_type_t = typename details::concat_type< Args... >::type;

// Creates a new tuple by concatenating several tuples

template< template< typename... > class T, typename... Args >
T< Args... > concat_tuples( T< Args... >& t )
{
    return t;
}

template< template< typename... > class T, typename... Args, typename... Others >
concat_type_t< T< Args... >, Others... > concat_tuples( T< Args... >& t, Others... others )
{
    auto prev = concat_tuples( others... );
    using CurrTuple = concat_type_t< T< Args... >, decltype( prev ) >;

    CurrTuple dest;
    copy_tuple( t, dest );
    copy_tuple< 0, sizeof...( Args ) >( prev, dest );

    return dest;
}

}

namespace details
{

template< typename... Tail >
struct concat_type;

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2, typename... TailArgs >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... >, TailArgs... >
{
    using type = typename concat_type< T< CurrArgs1..., CurrArgs2... >, TailArgs... >::type;
};

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2 >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... > >
{
    using type = class T< CurrArgs1..., CurrArgs2... >;
};

}

#endif
