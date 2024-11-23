#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <cstring>
#include <filesystem>

namespace SocketSysUtils
{
    constexpr size_t max_path_length()
    {
        return sizeof(sockaddr_un::sun_path);
    }

    inline const sockaddr* prepare_socket_address(sockaddr_un& s, const std::string path_to_socket)
    {
        memset(&s, 0, sizeof(sockaddr_un));
        s.sun_family = AF_UNIX;
        strncpy(s.sun_path, path_to_socket.c_str(), path_to_socket.length());
        return static_cast<const sockaddr*>(static_cast<void*>(&s));
    }

    inline std::pair<std::filesystem::path, std::error_code> path_to_socket_file()
    {
        std::error_code ec;
        std::filesystem::path path;
        path = std::filesystem::temp_directory_path(ec);
        if (!ec)
        {
            path /= "./socket_file";
        }
        return std::make_pair(path, ec);
    }
}
