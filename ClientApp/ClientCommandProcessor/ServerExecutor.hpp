#pragma once

#include "Command.hpp"
#include "../../Utils/Queue.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>


namespace SocketClient
{

class ClientConnection;

class ServerExecutor
{
public:

    ServerExecutor(std::shared_ptr<ClientConnection> client);
    ~ServerExecutor();

    void execute(std::unique_ptr<ClientCommands::ICommand>&& com);

private:
    void execute_impl();
    void execute_impl(const std::string& s);
    void init_thread();
    void responce(std::error_code ec);
    void responce(
        const std::string& responce_str, 
        std::unique_ptr<ClientCommands::ICommand>&& com
    );

    std::shared_ptr<ClientConnection> m_connection;
    
    std::thread m_thread;

    Utils::QueueWithWaiting<std::unique_ptr<ClientCommands::ICommand>> m_commands;

};

}