#include "ClientConnection.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/ErrorCodeDefinition.hpp"
#include "../Utils/SocketMessage.hpp"
#include "../Utils/SocketSysUtils.hpp"


using namespace SocketClient;
using namespace Utils;

std::error_code ClientConnection::connect(const std::string& path_to_socket_file)
{
    size_t path_length = path_to_socket_file.length() + 1; // last 0-code is need to take into account
    constexpr size_t max_path_len = SocketSysUtils::max_path_length();
    if (path_length > max_path_len)
    {
        Logger::singleton()("Path to socket file is too long, allowed max length ", sizeof(sockaddr::sa_data));
        return std::make_error_code(std::errc::filename_too_long);
    }

    m_id = socket(AF_UNIX, SOCK_STREAM, 0);
    if (!m_id)
    {
        std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
        Logger::singleton()("Socket not created. Error code: ", errno, " (", ec, ")");
        return ec;
    }

    Logger::singleton()("Socket is created - ", static_cast<int>(m_id));

    sockaddr_un addr;
    if (::connect(m_id, 
                  SocketSysUtils::prepare_socket_address(addr, path_to_socket_file), 
                  sizeof(addr)) < 0)
    {
        std::error_code ec = std::make_error_code(static_cast<std::errc>(errno));
        Logger::singleton()("connect error. Error code: ", errno, " (", ec, ")");
        return ec;
    }
    Logger::singleton()("Call 'connect' is succesful");
    return {};
}

std::error_code ClientConnection::write_message(const std::string& msg)
{
    return Utils::SocketMessage::write(m_id, msg);
}

std::tuple<std::error_code, std::string, bool> ClientConnection::read_message()
{
    return Utils::SocketMessage::read(m_id);
}
