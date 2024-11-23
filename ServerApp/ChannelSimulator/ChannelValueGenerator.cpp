#include "ChannelValueGenerator.hpp"
#include <thread>
#include <algorithm>


using namespace SocketApp::Channels;



//=============================================================
ChannelValueGenerator::ChannelValueGenerator(
    std::shared_ptr<Constants::ChannelsArray> channels,
    std::shared_ptr<std::mutex> mutex)
:m_channels(channels),
m_mutex(mutex),
m_rnd(std::chrono::steady_clock::now().time_since_epoch().count())
{
    
}

//=============================================================
void ChannelValueGenerator::run()
{
    while (execute_step())
    {
        std::this_thread::sleep_for(Constants::sleep_duration);
    }
}


//=============================================================
bool ChannelValueGenerator::execute_step()
{
    std::unique_lock<std::mutex> lock(*m_mutex);
    m_var.wait(lock, [&bstop=m_stop, &obj1 = m_started_channels, &obj2 = m_postponed_channels]() { 
        return bstop || !obj1.empty() || !obj2.empty(); 
    });
    if (m_stop)
        return false;

    restore_postponed_channels();
    auto t_now = std::chrono::high_resolution_clock().now();
    std::unordered_set<int> postponned_channels;
    for (int i : m_started_channels)
    {
        SocketApp::ChannelStatus s = generate_status();
        (*m_channels)[i].update(s, generate_value((*m_channels)[i].range()));

        if (s == SocketApp::ChannelStatus::Busy)
        {
            postponned_channels.insert(i);
            m_postponed_channels.emplace_back(i, t_now + Constants::busy_state_duration);
        }
        else if (s == SocketApp::ChannelStatus::Error)
        {
            postponned_channels.insert(i);
            m_postponed_channels.emplace_back(i, t_now + Constants::error_state_duration);
        }
    }
    return !m_stop;
}

//=============================================================
SocketApp::ChannelStatus ChannelValueGenerator::generate_status()
{
    std::uniform_int_distribution<int> distr(0, 100);
    int val = distr(m_rnd);
    if (val < Constants::error_state_persentage)
    {
        return SocketApp::ChannelStatus::Error;
    }
    else if (val > 100 - Constants::busy_state_persentage)
    {
        return SocketApp::ChannelStatus::Busy;
    }
    else
    {
        return SocketApp::ChannelStatus::Measure;
    }
}

//=============================================================
void ChannelValueGenerator::stop_all(int connection_id)
{
    std::lock_guard lock(*m_mutex);
    for (size_t i = 0; i < m_channels->size(); ++i)
    {
        stop_impl(i, connection_id);
    }

}

//=============================================================
void ChannelValueGenerator::stop_all()
{
    m_stop = true;
    m_var.notify_one();
}

//=============================================================
float ChannelValueGenerator::generate_value(SocketApp::ChannelRange r)
{
    std::uniform_real_distribution<float> distr(SocketApp::low_limit(r), 
                                                SocketApp::high_limit(r));
    return distr(m_rnd);
}

//=============================================================
void ChannelValueGenerator::restore_postponed_channels()
{
    if (!m_postponed_channels.empty())
    {
        auto t_now = std::chrono::high_resolution_clock().now();
        auto it = std::remove_if(m_postponed_channels.begin(),
                                 m_postponed_channels.end(),
                                 [t_now, &set = m_started_channels] (const auto& pair) {
                                    if (pair.second <= t_now)
                                    {
                                        set.insert(pair.first);
                                        return true;
                                    }
                                    return false;
                                 });

        m_postponed_channels.erase(it, m_postponed_channels.end());
    }
}

//=============================================================
void ChannelValueGenerator::start(int ichannel, int connection_id)
{
    std::lock_guard lock(*m_mutex);
    if ((*m_channels)[ichannel].start_measure(connection_id))
    {
        m_started_channels.insert(ichannel);
        m_var.notify_one();
    }
}

//=============================================================
void ChannelValueGenerator::stop(int ichannel, int connection_id)
{
    std::lock_guard lock(*m_mutex);
    stop_impl(ichannel, connection_id);
}

//=============================================================
void ChannelValueGenerator::stop_impl(int ichannel, int connection_id)
{
    if ((*m_channels)[ichannel].stop_measure(connection_id))
    {
        m_started_channels.erase(ichannel);
        auto it = std::find_if(m_postponed_channels.begin(), m_postponed_channels.end(),
            [i = ichannel](const auto& pair) { return pair.first == i; });
        if (it != m_postponed_channels.end())
            m_postponed_channels.erase(it);
    }
}

//=============================================================
ChannelValueGenerator::~ChannelValueGenerator()
{

}
