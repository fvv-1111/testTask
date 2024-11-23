#pragma once

#include "Connection.hpp"
#include "../Utils/Queue.hpp"
#include "../Utils/SocketHandle.hpp"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <sys/types.h>


namespace ServerApp
{

namespace ConnectionsConst
{
    constexpr const int thread_number = 4;
    constexpr const std::chrono::milliseconds sleep_time(10);
}

class Connections
{
public:
    Connections();

    void add_connection(SocketApp::SocketHandle&& h);

    void listen_attached_connections();

    void close_connection(int id);

    void restore_listen_connection(int id);

    void close_all();

    ~Connections();
private:
    bool prepare_fd_set(fd_set& tmp_set, int& max_val);
    void start_read_data(fd_set& set);
    void init_reading_threads();

    // thread to listen connections by function select()
    std::thread m_listen_thread;
    int m_listen_connection_counter = 0;
    bool m_stop = false;

    fd_set m_fdset;
    int m_max_value = 0;

    std::mutex m_mutex;
    std::condition_variable m_connection_wait;

    std::vector<std::shared_ptr<Connection>> m_connections;

    Utils::QueueWithWaiting<std::weak_ptr<Connection>> m_reading_data_queue;

    // threads for read data from connections and write responce
    std::vector<std::thread> m_reading_threads;
};

}