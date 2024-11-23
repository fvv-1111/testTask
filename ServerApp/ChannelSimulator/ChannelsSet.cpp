#include "ChannelsSet.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"

#include <cassert>

using namespace SocketApp::Channels;


//=============================================================
ChannelsSet& ChannelsSet::singleton()
{
    static ChannelsSet set;
    return set;
}

//=============================================================
int ChannelsSet::channels_number() const
{
    return static_cast<int>(m_channels->size());
}

//=============================================================
std::error_code ChannelsSet::range(int ichannel, SocketApp::ChannelRange& range)const
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);

    std::lock_guard lock(*m_mutex);
    range = (*m_channels)[ichannel].range();
    return {};
}

//=============================================================
std::error_code ChannelsSet::value(int ichannel, int connection_id, float& data)const
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);

    std::lock_guard lock(*m_mutex);
    auto [status, value] = (*m_channels)[ichannel].get(connection_id);
    switch (status)
    {
        case SocketApp::ChannelStatus::Busy:
            return std::make_error_code(SocketApp::AppErrors::ChannelBusyStatus);
        case SocketApp::ChannelStatus::Error:
            return std::make_error_code(SocketApp::AppErrors::ChannelErrorStatus);
        case SocketApp::ChannelStatus::Idle:
            return std::make_error_code(SocketApp::AppErrors::ChannelIdleStatus);
        case SocketApp::ChannelStatus::Measure:
            data = value;
            break;
        default:
            assert(0);
    }
    return {};
}

//=============================================================
std::error_code ChannelsSet::status(int ichannel, int connection_id, SocketApp::ChannelStatus& data)const
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);

    std::lock_guard lock(*m_mutex);
    auto [status, value] = (*m_channels)[ichannel].get(connection_id);
    data = status;
    return {};
}

//=============================================================
std::error_code ChannelsSet::set_range(int ichannel, SocketApp::ChannelRange new_range, int connection_id)
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);

    std::lock_guard lock(*m_mutex);
    return (*m_channels)[ichannel].set_range(new_range, connection_id);
}

//=============================================================
void ChannelsSet::unlock_range(int connection_id)
{
    std::lock_guard lock(*m_mutex);
    for (auto& p : (*m_channels))
        p.unlock_range(connection_id);
}

//=============================================================
std::error_code ChannelsSet::start_measure(int ichannel, int connection_id)
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);
    m_generator->start(ichannel, connection_id);
    return {};
}

//=============================================================
std::error_code ChannelsSet::stop_measure(int ichannel, int connection_id)
{
    if (ichannel >= channels_number())
        return std::make_error_code(SocketApp::AppErrors::IncorrectChannelIndex);
    m_generator->stop(ichannel, connection_id);
    return {};
}

//=============================================================
void ChannelsSet::stop_measure(int connection_id)
{
    m_generator->stop_all(connection_id);
}

//=============================================================
ChannelsSet::~ChannelsSet()
{
    m_generator->stop_all();
    m_generator_thread.join();
}

//=============================================================
ChannelsSet::ChannelsSet()
:m_channels(new Constants::ChannelsArray()),
m_mutex(new std::mutex)
{
    m_generator = std::make_unique<ChannelValueGenerator>(m_channels, m_mutex);
    m_generator_thread = std::thread([gen = m_generator.get()] () { gen->run(); });
}
