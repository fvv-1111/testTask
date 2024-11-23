#include "Connection.hpp"
#include "./ServerCommandProcessor/CommandProcessor.hpp"
#include "../Utils/Logger.hpp"
#include "ChannelSimulator/ChannelsSet.hpp"
#include "../Utils/ErrorCodeDefinition.hpp"
#include <unistd.h>
#include "../Utils/SocketMessage.hpp"
#include "Connections.hpp"


using namespace ServerApp;
using namespace Utils;


Connection::Connection(
    SocketApp::SocketHandle&& id, 
    Connections& parent
)
:m_id(std::move(id)),
m_parent(parent)
{
    Logger::singleton()("Connection ", static_cast<int>(m_id), " created");
}

Connection::~Connection()
{
    Logger::singleton()("Connection ", static_cast<int>(m_id), " closed");
    SocketApp::Channels::ChannelsSet::singleton().unlock_range(m_id);
    SocketApp::Channels::ChannelsSet::singleton().stop_measure(m_id);
}

void Connection::process_command(CommandProcessor::Processor& com_proc)
{
    Logger::singleton()("Connection ", static_cast<int>(m_id), "Start reading");

    auto [ec, command_str, connection_closed] = Utils::SocketMessage::read(m_id);
    if (connection_closed)
    {
        m_parent.close_connection(m_id);
        return;
    }
    if (ec)
    {
        Logger::singleton()("Connection ", static_cast<int>(m_id), "error reading :",
                            " ", ec.message());

        // I DO NOT SURE WHAT It IS NECESSARY TO DO in case error in reading/writing socket!!!!
    }
    else
    {
        Logger::singleton()("Connection ", static_cast<int>(m_id), " read command: ", command_str);

        std::string responce_str = com_proc.execute(command_str, *this);

        ec = Utils::SocketMessage::write(m_id, responce_str);
        if (ec)
        {
            Logger::singleton()("Connection ", static_cast<int>(m_id),
                                " error write responce: ", ec.message());

            // I DO NOT SURE WHAT It IS NECESSARY TO DO in case error in reading/writing socket!!!!

        }
        else
        {
            Logger::singleton()("Connection ", static_cast<int>(m_id), " write respone : ", responce_str);
        }
    }

    m_parent.restore_listen_connection(m_id);

}

