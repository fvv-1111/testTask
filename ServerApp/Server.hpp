//
// Server - class for craete socket and listen connections to it
//
#pragma once

#include <memory>

#include "../Utils/SocketHandle.hpp"
#include "Connections.hpp"

namespace ServerApp
{

class Connections;

class Server
{
public:
    static void start(const std::string& path_to_socket_file);

    ~Server();
private:
    void start_impl(const std::string& path_to_socket_file);

private:
    Connections m_connections;
    SocketApp::SocketHandle m_socket_id;
};

}