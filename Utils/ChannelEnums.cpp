#include "ChannelEnums.hpp"
#include <sstream>
#include <cassert>
#include <algorithm>

namespace SocketApp
{

std::string to_string(const ChannelRange r)
{
    std::stringstream ss;
    ss << "range" << static_cast<int>(r);
    return ss.str();
};

std::string to_string(const ChannelStatus s)
{
    switch ((s))
    {
    case ChannelStatus::Error:
        return "error";
    case ChannelStatus::Idle:
        return "idle";
    case ChannelStatus::Measure:
        return "measure";
    case ChannelStatus::Busy:
        return "busy";
    default:
        assert(0);
        return {};
    }
}

std::optional<ChannelRange> range_from_int(int v)
{
    if (v >= static_cast<int>(ChannelRange::Range0) && v <= static_cast<int>(ChannelRange::Range3))
        return static_cast<ChannelRange>(v);
    return {};
}

float low_limit(const ChannelRange r)
{
    switch (r)
    {
        case ChannelRange::Range0:
            return 0.0000001f;
        case ChannelRange::Range1:
            return 0.0001f;
        case ChannelRange::Range2:
            return 1.0f;
        case ChannelRange::Range3:
            return 1000.0f;
        default:
            assert(0);
            return 0.0000001f;
    }
}

float high_limit(const ChannelRange r)
{
    switch (r)
    {
        case ChannelRange::Range0:
            return 0.0001f;
        case ChannelRange::Range1:
            return 1.0f;
        case ChannelRange::Range2:
            return 1000.0f;
        case ChannelRange::Range3:
            return 1000000.0f;
        default:
            assert(0);
            return 0.0001f;
    }

}

}