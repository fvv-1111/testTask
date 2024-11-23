#include "GetResultCommand.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../Connection.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"

#include "../../Utils/Logger.hpp"

using namespace ServerApp::CommandProcessor;
using namespace Utils::CommandProcessor::Command;
using namespace Utils;


void GetResultCommand::execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj)
{
    int ichannel = GetResultRegex::channel_index(match_obj);
    float value;
    std::error_code ec = SocketApp::Channels::ChannelsSet::singleton().value(
        ichannel, conn_obj.connection_id(), value);
    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get result command - channel", ichannel, ":", value);
        responce(responce_stream, ichannel, value);
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get result command - error in channel", ichannel, ":", ec);
        responce(responce_stream, ichannel, ec);
    }
}


