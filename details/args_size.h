#ifndef ARGS_SIZE_H
#define ARGS_SIZE_H

namespace details
{

template< typename... Args >
constexpr size_t args_size()
{
    return sizeof...(Args);
}

}

#endif
