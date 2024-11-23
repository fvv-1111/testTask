#pragma once

#include <mutex>
#include <system_error>
#include <utility>
#include <unordered_set>
#include "../../Utils/ChannelEnums.hpp"
#include "../../Utils/SocketHandle.hpp"

namespace SocketApp
{
namespace Channels
{

    class Channel
    {
    public:
        Channel();
        Channel(Channel&&);
        void operator=(Channel&&);
        ~Channel();

        SocketApp::ChannelRange range()const;
        std::pair<SocketApp::ChannelStatus,float> get(int connection_id)const;

        std::error_code set_range(SocketApp::ChannelRange new_range, int connection_id);
        void unlock_range(int connection_id);

        void update(SocketApp::ChannelStatus new_status, float normalized_value);

        bool start_measure(int connection_id);
        bool stop_measure(int connection_id);
        bool is_measure_started()const;
        bool is_measure_started(int connection_id)const;
    private:
        SocketApp::ChannelStatus m_status = SocketApp::ChannelStatus::Idle;
        float m_value = 0;
        SocketApp::ChannelRange m_range = SocketApp::ChannelRange::Range0;
        std::unordered_set<int> m_started_connetion_ids;

        // id of connection which locked range value
        int m_connect_id = SocketApp::SocketHandleConst::invalid_socket_id;

        // no copy
        Channel(const Channel&&) = delete;
        void operator=(const Channel&) = delete;
    };
    
}
}