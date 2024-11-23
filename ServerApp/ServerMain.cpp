#include <iostream>
#include "Server.hpp"
#include "../Utils/SocketSysUtils.hpp"

int main()
{

    auto [path, dir_error] = SocketSysUtils::path_to_socket_file();
    if (dir_error)
    {
        Utils::Logger::singleton()("Error to get tmp directory for socket file", dir_error);
        return 1;
    }
    Utils::Logger::singleton()(path);

    ServerApp::Server::start(path);
}