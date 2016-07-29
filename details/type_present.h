#ifndef TUPLE_TYPE_PRESENT_H
#define TUPLE_TYPE_PRESENT_H

#include <type_traits>

namespace basic
{

// Checks if Target type is among the listed

template< typename Target, typename Curr, typename... Tail>
struct type_present : std::conditional< std::is_same< Target, Curr >::value,
                                        std::true_type,
                                        type_present< Target, Tail... > >::type
{

};

template< typename Target, typename Curr >
struct type_present< Target, Curr > : std::conditional< std::is_same< Target, Curr >::value,
                                                        std::true_type,
                                                        std::false_type >::type
{

};

// Convenience alias

template< typename Target, template< typename... > class T, typename... Args >
constexpr bool type_present_t( const T< Args... >& t )
{
    return type_present< Target, Args... >::value;
}

template< typename Target, template< typename... > class T, typename... Args >
constexpr bool type_present_t( const T< Args... >&& t )
{
    return type_present< Target, Args... >::value;
}

}

#endif
