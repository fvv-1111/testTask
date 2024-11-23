#include "ServerExecutor.hpp"
#include "../ClientConnection.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"
#include "../ClientApp.hpp"
#include "ResponceEvent.hpp"

using namespace SocketClient;


ServerExecutor::ServerExecutor(std::shared_ptr<ClientConnection> client)
:m_connection(client)
{
    init_thread();
}

ServerExecutor::~ServerExecutor()
{
    m_commands.stop();
    m_thread.join();
}

void ServerExecutor::execute(std::unique_ptr<ClientCommands::ICommand>&& com)
{
    m_commands.push(std::move(com));
}

void ServerExecutor::execute_impl()
{
    std::unique_ptr<ClientCommands::ICommand> pcom;
    while (m_commands.pop(pcom))
    {
        std::string msg = pcom->get_server_command_string();
        std::error_code ec = m_connection->write_message(msg);
        if (ec)
        {
            responce(ec);
        }
        else
        {
            auto [ec1, str, closed_connection] = m_connection->read_message();
            if (closed_connection)
            {
                responce(std::make_error_code(SocketApp::AppErrors::ConnectionWithServerBroken));
            }
            else if (ec1)
            {
                responce(ec1);
            }
            else
            {
                responce(str, std::move(pcom));
            }
        }
    }
}

void ServerExecutor::responce(std::error_code ec)
{
    ClientCommands::ResponceEvent* pevent = new ClientCommands::ResponceEvent(ec);
    QApplication::postEvent(ClientApp::ClientApp::singleton(), pevent);
}

void ServerExecutor::responce(
    const std::string& responce_str, 
    std::unique_ptr<ClientCommands::ICommand>&& com
)
{
    ClientCommands::ResponceEvent* pevent = new ClientCommands::ResponceEvent(std::move(com), responce_str);
    QApplication::postEvent(ClientApp::singleton(), pevent);
}


void ServerExecutor::init_thread()
{
    m_thread = std::thread([self = this] () {
        self->execute_impl();
    });
}
