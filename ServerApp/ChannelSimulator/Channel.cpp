#include "Channel.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"

using namespace SocketApp::Channels;

Channel::Channel()
{

}

Channel::Channel(Channel&& c)
:m_status(c.m_status),
m_value(c.m_value),
m_range(c.m_range),
m_started_connetion_ids(std::move(c.m_started_connetion_ids)),
m_connect_id(c.m_connect_id)
{
    c.m_connect_id = SocketApp::SocketHandleConst::invalid_socket_id;
}

void Channel::operator=(Channel&& c)
{
    if (this == &c)
        return;

    m_connect_id = c.m_connect_id;
    m_started_connetion_ids = std::move(c.m_started_connetion_ids);
    m_status = c.m_status;
    m_value = c.m_value;
    m_range = c.m_range;
    c.m_connect_id = SocketApp::SocketHandleConst::invalid_socket_id;
}

Channel::~Channel()
{

}

SocketApp::ChannelRange Channel::range()const
{
    return m_range;
}

std::pair<SocketApp::ChannelStatus,float> Channel::get(int connection_id)const
{
    if (is_measure_started(connection_id))
        return std::make_pair(m_status, m_value);
    else
        return std::make_pair(SocketApp::ChannelStatus::Idle, 0);
}

std::error_code Channel::set_range(SocketApp::ChannelRange new_range, int connection_id)
{
    if (is_measure_started())
    {
        return std::make_error_code(SocketApp::AppErrors::RangeLockedByChannelStarted);
    }
    if (m_connect_id == connection_id || SocketApp::SocketHandleConst::is_invalid(m_connect_id))
    {
        m_connect_id = connection_id;
        m_range = new_range;
        return {};
    }
    else
    {
        return std::make_error_code(SocketApp::AppErrors::RangeWasLockedAnotherUser);
    }
}

void Channel::unlock_range(int connection_id)
{
    if (m_connect_id == connection_id)
        SocketApp::SocketHandleConst::set_invalid(m_connect_id);
}

void Channel::update(SocketApp::ChannelStatus new_status, float value)
{
    if (!is_measure_started())
        return;
    
    m_status = new_status;
    m_value = value;
}

bool Channel::start_measure(int connection_id)
{
    bool really_started = m_started_connetion_ids.empty();
    m_started_connetion_ids.insert(connection_id);
    return really_started;
}

bool Channel::stop_measure(int connection_id)
{
    if (m_started_connetion_ids.empty())
        return false;
    m_started_connetion_ids.erase(connection_id);
    return m_started_connetion_ids.empty();
}

bool Channel::is_measure_started()const
{
    return !m_started_connetion_ids.empty();
}

bool Channel::is_measure_started(int connection_id)const
{
    return m_started_connetion_ids.find(connection_id) != m_started_connetion_ids.end();
}
