#include "ClientApp.hpp"
#include "ClientConnection.hpp"
#include "ClientCommandProcessor/ServerExecutor.hpp"
#include "ClientCommandProcessor/ResponceEvent.hpp"


using namespace SocketClient;

ClientApp::ClientApp(int &argc, char **argv)
:QApplication(argc, argv)
{
    m_client_connection = std::make_shared<ClientConnection>();
    m_server_executor = std::make_unique<ServerExecutor>(m_client_connection);

    QObject::connect(this, SIGNAL(applicationStateChanged(Qt::ApplicationState)),
                     this, SLOT(applicationStateChanged(Qt::ApplicationState)));
}

ClientApp::~ClientApp()
{

}

std::error_code ClientApp::connect_to_server(const std::string& path)
{
    std::error_code ec = m_client_connection->connect(path);
    if (ec)
        emit server_operation_error(ec);
    else
        emit server_connected();
    return ec;
}

bool ClientApp::event(QEvent*e)
{
    if (e->type() == ClientCommands::ResponceEvent::event_type())
    {
        ClientCommands::ResponceEvent* event = static_cast<ClientCommands::ResponceEvent*>(e);
        if (event->error_code())
        {
            emit server_operation_error(event->error_code());
        }
        else
        {
            event->execute_responce();
        }
        return true;
    }
    else
        return QApplication::event(e);
}

void ClientApp::applicationStateChanged(Qt::ApplicationState state)
{
    static bool done = false;
    if (!done && state == Qt::ApplicationState::ApplicationActive)
    {
        done = true;
        connect_to_server(m_path_to_socket);
    }
}

void ClientApp::set_socket_path(const std::string& path)
{
    m_path_to_socket = path;
}

void ClientApp::execute_command(std::unique_ptr<ClientCommands::ICommand>&& com)
{
    m_server_executor->execute(std::move(com));
}

GetResultsProcess& ClientApp::results_process()
{
    return m_results_process;
}
