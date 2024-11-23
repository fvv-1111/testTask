#include "ChannelData.hpp"
#include "../../../Utils/ErrorCodeDefinition.hpp"
#include <system_error>

using namespace MVC::Model;

ChannelData::ChannelData(int ichannel)
:m_ichannel(ichannel)
{

}

ChannelData::~ChannelData()
{

}

void ChannelData::start_measure(std::error_code ec)
{
    m_error = ec;
    m_measure_is_started = true;
    emit update_channel_start_stop_status(m_ichannel, m_measure_is_started, ec);
}

void ChannelData::stop_measure(std::error_code ec)
{
    m_error = ec;
    m_measure_is_started = false;
    emit update_channel_start_stop_status(m_ichannel, m_measure_is_started, ec);
}

void ChannelData::set_value(float v)
{
    m_error = {};
    m_value = v;
    m_status = SocketApp::ChannelStatus::Measure;
    emit update_channel_value_and_status(m_ichannel, m_value, m_status, m_error);
}

void ChannelData::set_value(std::error_code ec)
{
    m_error = ec;
    if (ec == std::make_error_code(SocketApp::AppErrors::ChannelErrorStatus))
    {
        m_status = SocketApp::ChannelStatus::Error;
        m_error = {};
    }
    else if (ec == std::make_error_code(SocketApp::AppErrors::ChannelBusyStatus))
    {
        m_status = SocketApp::ChannelStatus::Busy;
        m_error = {};
    }
    else if (ec == std::make_error_code(SocketApp::AppErrors::ChannelIdleStatus))
    {
        m_status = SocketApp::ChannelStatus::Idle;
        m_error = {};
    }
    else
    {
        m_status = SocketApp::ChannelStatus::Measure;
        m_error = {};
    }
    emit update_channel_value_and_status(m_ichannel, m_value, m_status, m_error);
}

void ChannelData::set_status(SocketApp::ChannelStatus s)
{
    m_error = {};
    m_status = s;
    emit update_channel_value_and_status(m_ichannel, m_value, m_status, m_error);
}

void ChannelData::set_status(std::error_code ec)
{
    m_error = ec;
    emit update_channel_value_and_status(m_ichannel, m_value, m_status, m_error);
}

void ChannelData::set_range(SocketApp::ChannelRange r, std::error_code ec)
{
    m_error = ec;
    m_range = r;
    emit update_channel_range(m_ichannel, m_range, ec);
}

void ChannelData::set_range(SocketApp::ChannelRange r)
{
    m_error = {};
    m_range = r;
    emit update_channel_range(m_ichannel, m_range, m_error);
}

void ChannelData::set_range(std::error_code ec)
{
    m_error = ec;
    emit update_channel_range(m_ichannel, m_range, m_error);
}

bool ChannelData::is_measure_started()const
{
    return m_measure_is_started;
}

float ChannelData::value()const
{
    return m_value;
}

SocketApp::ChannelRange ChannelData::range()const
{
    return m_range;
}

SocketApp::ChannelStatus ChannelData::status()const
{
    return m_status;
}

std::error_code ChannelData::error()const
{
    return m_error;
}
