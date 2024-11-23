#include <iostream>
#include "ClientApp.hpp"

#include "MVC/Controller/ViewController.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/SocketSysUtils.hpp"


int main(int argc, char *argv[])
{

    auto [path_to_socket_file, dir_error] = SocketSysUtils::path_to_socket_file();
    if (dir_error)
    {
        Utils::Logger::singleton()("Error to get tmp directory for socket file", dir_error);
        return 1;
    }
    Utils::Logger::singleton()(path_to_socket_file);

    SocketClient::ClientApp app(argc, argv);

    MVC::ViewController vc;
    vc.show();

    app.set_socket_path(path_to_socket_file);

    return app.exec();
}