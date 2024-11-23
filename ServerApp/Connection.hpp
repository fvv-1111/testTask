//
// Class for store data for one connection - for example, indices of channels which was started, selected ranges for channel.
//

#pragma once

#include <mutex>
#include <unordered_map>
#include <set>
#include <optional>
#include "../Utils/ChannelEnums.hpp"
#include "../Utils/SocketHandle.hpp"
#include "ServerCommandProcessor/CommandProcessor.hpp"
#include <system_error>


namespace ServerApp
{

class Connections;


class Connection
{
public:
    Connection(SocketApp::SocketHandle&& id, 
               Connections& parent);

    ~Connection();

    void process_command(CommandProcessor::Processor& com_proc);

    inline int connection_id() const
    {
        return m_id;
    }
private:

    // no copy
    Connection() = delete;
    Connection(const Connection&) = delete;
    void operator =(const Connection&) = delete;
    Connection(Connection&&) = delete;
    void operator =(Connection&&) = delete;

    SocketApp::SocketHandle m_id;
    Connections& m_parent;

};

}