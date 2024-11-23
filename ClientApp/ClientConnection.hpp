#pragma once

#include "../Utils/SocketHandle.hpp"

#include <system_error>
#include <string>
#include <tuple>


namespace SocketClient
{

class ClientConnection
{
public:
    std::error_code connect(const std::string& path_to_socket_file);

    std::error_code write_message(const std::string& msg);

    std::tuple<std::error_code, std::string, bool> read_message();
private:
    SocketApp::SocketHandle m_id;
};

}