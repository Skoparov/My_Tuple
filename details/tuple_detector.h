#ifndef TUPLE_DETECTOR_H
#define TUPLE_DETECTOR_H

namespace basic
{

struct base_tuple{};

template<class T, class R = void>
struct enable_if_type { typedef R type; };

template<class T, class Enable = void>
struct is_tuple : std::false_type {};

template<class T>
struct is_tuple<T, typename enable_if_type<typename T::tuple_tag>::type> : std::true_type{};

}

#endif
