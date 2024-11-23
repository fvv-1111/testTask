//
// Definition of necessary enums type for channels
//
#pragma once

#include <string>
#include <optional>

namespace SocketApp
{

enum class ChannelRange: char
{
    Range0,
    Range1,
    Range2,
    Range3
};

extern std::string to_string(const ChannelRange);
extern std::optional<ChannelRange> range_from_int(int);
extern float low_limit(const ChannelRange);
extern float high_limit(const ChannelRange);



enum class ChannelStatus: char
{
    Error,
    Idle,
    Measure,
    Busy
};

std::string to_string(const ChannelStatus);

template<typename T>
inline size_t size() 
{
    static_assert(sizeof(T) == -1, "specialization must be used.");
    return 0;
}

template<>
inline size_t size<ChannelRange>() 
{
    return 4;
}

template<>
inline size_t size<ChannelStatus>() 
{
    return 4;
}

template<typename T>
inline int size_int() 
{
    return static_cast<int>(size<T>());
}
}