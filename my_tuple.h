#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include <type_traits>

namespace details
{

template< typename... Tail >
struct concat_type;

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2, typename... TailArgs >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... >, TailArgs... >;

template< template< typename... > class T, typename... CurrArgs1, typename... CurrArgs2 >
struct concat_type< T< CurrArgs1... >, T< CurrArgs2... > >;

template< size_t k, typename T, typename... Tail >
struct elem_type;

template< typename T, typename... Tail >
struct elem_type< 0, T, Tail... >;

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( T< Args1... >& source, T< Args2... >& target );

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( T< Args1... >&, T< Args2... >& );

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

// Acquires the type of several tuples' concatenation result

template< typename... Args >
using concat_type_t = typename details::concat_type< Args... >::type;

// Copies the elements of one tuple to another

template< size_t begin = 0, size_t destBegin = 0, size_t count = 0,
          template< typename... > class T, typename... Args1, typename... Args2 >
void copy_tuple( T< Args1... >& source, T< Args2... >& target )
{
    static_assert( begin >= 0, "Begin position must be >= 0" );
    static_assert( destBegin >= 0, "Dest begin position must be >= 0" );
    static_assert( count >= 0, "Number of objects to be copied must be >= 0" );

    constexpr size_t actualCount = count > 0? count : sizeof...( Args1 );
    static_assert( actualCount <= sizeof...( Args2 ), "Number of objects to be copied must be <= dest dize" );

    return details::copy_tuple< begin, destBegin, actualCount >( source, target );
}

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

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count != 0 >* = nullptr >
void copy_tuple( T< Args1... >& source, T< Args2... >& target )
{
    basic::get< destIndex >(target) = basic::get< index >( source );
    return details::copy_tuple< index + 1, destIndex + 1, count - 1 >( source, target );
}

template< size_t index, size_t destIndex, size_t count, template< typename... > class T, typename... Args1, typename... Args2,
          std::enable_if_t< count == 0 >* = nullptr >
void copy_tuple( T< Args1... >&, T< Args2... >& )
{

}

}

#endif
