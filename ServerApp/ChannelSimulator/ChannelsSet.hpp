#pragma once

#include <memory>
#include "Channel.hpp"
#include "ChannelValueGenerator.hpp"
#include <thread>
#include <utility>
#include "ChannelsConst.hpp"

namespace SocketApp
{
namespace Channels
{

class ChannelsSet
{
public:
    static ChannelsSet& singleton();

    int channels_number() const;

    std::error_code range(int ichannel, SocketApp::ChannelRange& range)const;
    std::error_code value(int ichannel, int connection_id, float& data)const;
    std::error_code status(int ichannel, int connection_id, SocketApp::ChannelStatus& data)const;

    std::error_code set_range(int ichannel, SocketApp::ChannelRange new_range, int connection_id);

    // Note: this method is called only from connection object destructor 
    void unlock_range(int connection_id);

    std::error_code start_measure(int ichannel, int connection_id);
    std::error_code stop_measure(int ichannel, int connection_id);

    // this call when connection becomes broken.
    void stop_measure(int connection_id);


    ~ChannelsSet();
private:
    ChannelsSet();

    std::shared_ptr<Constants::ChannelsArray> m_channels;
    std::shared_ptr<std::mutex> m_mutex;

    std::unique_ptr<ChannelValueGenerator> m_generator;
    std::thread m_generator_thread;


    // no copy
    ChannelsSet(const ChannelsSet&&) = delete;
    void operator=(const ChannelsSet&) = delete;
    ChannelsSet(ChannelsSet&&) = delete;
    void operator=(ChannelsSet&&) = delete;
};

}
}