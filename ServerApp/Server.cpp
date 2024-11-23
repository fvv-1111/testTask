#include <mutex>
#include <thread>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <filesystem>


#include "../Utils/Logger.hpp"
#include "../Utils/SocketSysUtils.hpp"
#include "Server.hpp"

using namespace ServerApp;
using namespace Utils;


constexpr int number_requests_in_socket_queue = 64;

//============================================
void Server::start(const std::string& path_to_socket_file)
{
    Server s;
    s.start_impl(path_to_socket_file);
}

//============================================
void Server::start_impl(const std::string& path_to_socket_file)
{
    Logger::singleton()("Path to socket:", path_to_socket_file);
    size_t path_length = path_to_socket_file.length() + 1; // last 0-code is need to take into account
    constexpr size_t max_path_len = SocketSysUtils::max_path_length();
    if (path_length > max_path_len)
    {
        Logger::singleton()("Path to socket file is too long, allowed max length ", max_path_len);
        return;
    }

    std::error_code create_dir_err_code;
    std::filesystem::path dirs = std::filesystem::path(path_to_socket_file).parent_path();
    Logger::singleton()(dirs);
    if (!std::filesystem::create_directories(dirs, create_dir_err_code))
    {
        if (create_dir_err_code)
        {
            Logger::singleton()("Error to create directiries for socket file ", create_dir_err_code.message());
            return;
        }
    }

    if (unlink(path_to_socket_file.c_str()) != 0)
    {
        if (errno != static_cast<int>(std::errc::no_such_file_or_directory))
            Logger::singleton()("unlink error. Error code: ", errno, " (", Logger::last_error(), ")");
    }

    // create socket Unix Domain Socket. Third arg (protocol) is always 0 for AF_UNIX socket.
    m_socket_id = socket(AF_UNIX, SOCK_STREAM, 0);

    if (!m_socket_id)
    {
        Logger::singleton()("Socket not created. Error code: ", errno, " (", Logger::last_error(), ")");
        return;
    }

    Logger::singleton()("Socket is created - ", static_cast<int>(m_socket_id));

    sockaddr_un addr;
    if (bind(m_socket_id, 
             SocketSysUtils::prepare_socket_address(addr, path_to_socket_file), 
             sizeof(sockaddr_un)) < 0)
    {
        Logger::singleton()("Bind error. Error code: ", errno, " (", Logger::last_error(), ")");
        return;
    }
    Logger::singleton()("Call 'bind' is succesful");

    if (listen(m_socket_id, number_requests_in_socket_queue) < 0)
    {
        Logger::singleton()("Listen error. Error code: ", errno, " (", Logger::last_error(), ")");
        return;
    }
    Logger::singleton()("Call 'listen' is succesful");

    while(true)
    {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(m_socket_id, &fdset);
        _STRUCT_TIMEVAL wait_time;
        wait_time.tv_sec = 0;
        wait_time.tv_usec = 10000; // 1ms
        int max_val = static_cast<int>(m_socket_id) + 1;
        int ret_code = select(max_val, &fdset, nullptr, nullptr, &wait_time);
        
        if (ret_code < 0)
        {
            Logger::singleton()("select error. Error code: ", errno, " (", Logger::last_error(), ")");
        }
        else if(ret_code > 0)
        {
            Logger::singleton()("New connection is observed by 'select'-function");
            SocketApp::SocketHandle connection_id = accept(m_socket_id, nullptr, nullptr);
            if (connection_id)
            {
                Logger::singleton()("New connection is accepted ", static_cast<int>(connection_id));
                m_connections.add_connection(std::move(connection_id));
            }
            else
            {
                Logger::singleton()("accept connection error. Error code: ", errno, " (", Logger::last_error(), ")");
            }
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

Server::~Server()
{
    m_connections.close_all();
}
