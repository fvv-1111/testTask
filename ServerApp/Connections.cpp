#include "Connections.hpp"
#include "ServerCommandProcessor/CommandProcessor.hpp"
#include "../Utils/Logger.hpp"

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <algorithm>
#include <cstring>

using namespace ServerApp;
using namespace Utils;

Connections::Connections()
{
    m_listen_thread = std::thread([](Connections* p) { p->listen_attached_connections(); }, this);
    init_reading_threads();
    FD_ZERO(&m_fdset);
}

Connections::~Connections()
{
    close_all();
}

void Connections::close_all()
{

    {
        std::lock_guard lock(m_mutex);
        if (m_stop)
            return;
        m_stop = true;
        m_reading_data_queue.stop();
    }
    m_connection_wait.notify_one();
    if (m_listen_thread.joinable())
        m_listen_thread.join();
    for (auto& t: m_reading_threads)
    {
        if (t.joinable())
            t.join();
    }
    m_connections.clear();
}

void Connections::add_connection(SocketApp::SocketHandle&& h)
{
    int i = h;
    {
        std::lock_guard lock(m_mutex);
        if (m_stop)
            return;
        m_connections.emplace_back(new Connection(std::move(h), *this));
    }
    restore_listen_connection(i);
}

void Connections::listen_attached_connections()
{
    fd_set tmp_set;
    int max_id;
    while (prepare_fd_set(tmp_set, max_id))
    {
        _STRUCT_TIMEVAL wait_time;
        wait_time.tv_sec = 0;
        wait_time.tv_usec = 10000; // 10ms
        int ret_code = select(max_id, &tmp_set, nullptr, nullptr, &wait_time);
        if (ret_code > 0)
            start_read_data(tmp_set);
        std::this_thread::yield();
    }
}

void Connections::close_connection(int id)
{
    std::lock_guard lock(m_mutex);
    if (m_stop)
        return;
    auto&& it = std::find_if(m_connections.begin(), m_connections.end(), 
        [id](auto&p) { return p->connection_id() == id; });
    if (it != m_connections.end())
    {   
        int id = (*it)->connection_id();
        if (FD_ISSET(id, &m_fdset))
        {
            FD_CLR(id, &m_fdset);
            --m_listen_connection_counter;
        }
        m_connections.erase(it);
    }
}

void Connections::restore_listen_connection(int id)
{
    std::lock_guard lock(m_mutex);
    if (!m_stop && !FD_ISSET(id, &m_fdset))
    {
        Logger::singleton()("Connection ", id, " restored to listen");
        
        FD_SET(id, &m_fdset);
        ++m_listen_connection_counter;
        m_max_value = std::max(m_max_value, id + 1);
        m_connection_wait.notify_one();
    }

}

bool Connections::prepare_fd_set(fd_set& tmp_set, int& max_val)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_connection_wait.wait(lock, [self=this](){
        return self->m_listen_connection_counter > 0 || self->m_stop; 
    });

    if (!m_stop)
    {
        memcpy(&tmp_set, &m_fdset, sizeof(m_fdset));
        max_val = m_max_value;
    }
    return !m_stop;
}

void Connections::start_read_data(fd_set& set)
{
    std::lock_guard lock(m_mutex);
    if (m_stop)
        return;
    for (auto& connecton : m_connections)
    {
        if (FD_ISSET(connecton->connection_id(), &set))
        {
            Logger::singleton()("Connection ", static_cast<int>(connecton->connection_id()), " in queue to read command");

            FD_CLR(connecton->connection_id(), &m_fdset);
            m_reading_data_queue.emplace(connecton);
            --m_listen_connection_counter;
        }
    }
}

void Connections::init_reading_threads()
{

    for (int i = 0; i < ConnectionsConst::thread_number; ++i)
    {
        m_reading_threads.emplace_back( 
            [&queue = m_reading_data_queue]() {
                CommandProcessor::Processor proc;
                std::weak_ptr<Connection> p;
                while (queue.pop(p))
                {
                    if (std::shared_ptr<Connection> p1 = p.lock())
                    {
                        p1->process_command(proc);
                    }
                }
            });
    }
}
