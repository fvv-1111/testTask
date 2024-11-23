#include "SocketMessage.hpp"
#include "ErrorCodeDefinition.hpp"

#include "unistd.h"


using namespace Utils;


std::tuple<std::error_code, std::string, bool> SocketMessage::read(const SocketApp::SocketHandle& h)
{
    std::string msg = "";
    std::error_code ec;
    bool closed = false;

    constexpr size_t buff_len = 20;
    char buff[buff_len];
    int num_bytes_to_read = buff_len - 1;
    bool all_message_read = false;
    do {
        ssize_t len = ::read(h, buff, num_bytes_to_read);
        if (len == 0)
        {
            closed = true;
            break;
        }
        else if (len < 0)
        {
            ec = std::make_error_code(static_cast<std::errc>(errno));
            break;
        }
        else if (!closed)
        {
            buff[len] = 0;
            all_message_read = (len < num_bytes_to_read) || (buff[len - 1] == '\n');
            msg += buff;
        }
    } while (!all_message_read);
    return {ec, msg, closed};
}

std::error_code SocketMessage::write(const SocketApp::SocketHandle& h, const std::string& msg)
{
    std::string buff = msg;
    if (buff.empty() || buff[buff.length() - 1] != '\n')
        buff += '\n';
    ssize_t len = ::write(h, buff.c_str(), buff.length());
    if (len < 0)
            return std::make_error_code(static_cast<std::errc>(errno));
    return {};
}
