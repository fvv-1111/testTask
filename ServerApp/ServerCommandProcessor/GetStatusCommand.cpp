#include "GetStatusCommand.hpp"
#include "../Connection.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"
#include "../../Utils/Logger.hpp"


using namespace ServerApp::CommandProcessor;
using namespace Utils::CommandProcessor::Command;
using namespace Utils;


void GetStatusCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    Connection& conn_obj)
{
    int ichannel = GetStatusRegex::channel_index(match_obj);
    SocketApp::ChannelStatus status;
    std::error_code ec = SocketApp::Channels::ChannelsSet::singleton().status(
        ichannel, conn_obj.connection_id(), status);
    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get status command - channel", ichannel, ":", SocketApp::to_string(status));
        responce(responce_stream, ichannel, status);
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get status command - error in channel", ichannel, ":", ec);
        responce(responce_stream, ichannel, ec);
    }
}


