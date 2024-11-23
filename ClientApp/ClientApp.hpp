#pragma once

#include "ClientCommandProcessor/GetResultsProcess.hpp"
#include <QtWidgets/QApplication>
#include <system_error>
#include <memory>

namespace SocketClient
{

class ClientConnection;
class ServerExecutor;

namespace ClientCommands
{
    class ICommand;
}

class ClientApp: public QApplication
{
    Q_OBJECT
public:
    ClientApp(int &argc, char **argv);

    virtual ~ClientApp();

    static ClientApp* singleton()
    {
        return static_cast<ClientApp*>(QApplication::instance());
    }

    void execute_command(std::unique_ptr<ClientCommands::ICommand>&& com);

    void set_socket_path(const std::string& path);

    GetResultsProcess& results_process();

signals:
    void server_operation_error(std::error_code);
    void server_connected();

private slots:
    void applicationStateChanged(Qt::ApplicationState state);
protected:
    bool event(QEvent*e)override;

private:
    std::error_code connect_to_server(const std::string& path);

    std::shared_ptr<ClientConnection> m_client_connection;
    std::unique_ptr<ServerExecutor> m_server_executor;
    std::string m_path_to_socket;
    GetResultsProcess m_results_process;
};

}