#pragma once

#include <chrono>
#include <array>

namespace SocketApp
{
namespace Channels
{
class Channel;

namespace Constants
{
    constexpr const int error_state_persentage = 10;
    constexpr const int busy_state_persentage = 15;

    constexpr const std::chrono::seconds error_state_duration(3);
    constexpr const std::chrono::milliseconds busy_state_duration(1500);
    constexpr const std::chrono::milliseconds sleep_duration(5);

#ifndef NUM_CHANNELS
#define NUM_CHANNELS 8
#endif

    constexpr const int channels_number = NUM_CHANNELS;
    using ChannelsArray = std::array<Channel, channels_number>;

}
}
}
