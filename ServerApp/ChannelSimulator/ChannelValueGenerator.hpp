#pragma once

#include "Channel.hpp"
#include <vector>
#include <unordered_set>
#include <random>
#include <chrono>
#include <utility>
#include "ChannelsConst.hpp"

namespace SocketApp
{
namespace Channels
{


class ChannelValueGenerator
{
public:
    ChannelValueGenerator(std::shared_ptr<Constants::ChannelsArray> channels,
                          std::shared_ptr<std::mutex> mutex);

    void run();

    void start(int ichannel, int connection_id);
    void stop(int ichannel, int connection_id);

    void stop_all(int connection_id);
    void stop_all();

    ~ChannelValueGenerator();
private:
    using PostponedItem = std::pair<int, std::chrono::high_resolution_clock::time_point>;

    bool execute_step();
    void restore_postponed_channels();
    SocketApp::ChannelStatus generate_status();
    float generate_value(SocketApp::ChannelRange r);
    void stop_impl(int ichannel, int connection_id);


    std::shared_ptr<Constants::ChannelsArray> m_channels;
    std::shared_ptr<std::mutex> m_mutex;
    std::condition_variable m_var;
    bool m_stop = false;
    std::unordered_set<int> m_started_channels;
    std::vector<PostponedItem> m_postponed_channels;
    std::vector<int> m_internal_chache;
    std::mt19937 m_rnd;
    

    // not copy
    ChannelValueGenerator(const ChannelValueGenerator&) = delete;
    ChannelValueGenerator(ChannelValueGenerator&&) = delete;
    void operator=(const ChannelValueGenerator&) = delete;
    void operator=(ChannelValueGenerator&&) = delete;
    ChannelValueGenerator() = delete;
};

}
}