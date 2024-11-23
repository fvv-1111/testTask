#include "GetRangeCommand.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../Connection.hpp"
#include "../../Utils/ChannelEnums.hpp"
#include "../../Utils/Logger.hpp"

using namespace ServerApp::CommandProcessor;
using namespace Utils::CommandProcessor::Command;
using namespace Utils;


void GetRangeCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    ServerApp::Connection& conn_obj)
{
    int ichannel = GetRangeRegex::channel_index(match_obj);
    SocketApp::ChannelRange range;
    std::error_code ec = SocketApp::Channels::ChannelsSet::singleton().range(ichannel, range);

    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get range command - channel", ichannel, ":", SocketApp::to_string(range));
        responce(responce_stream, ichannel, range);
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " get range command - error in channel", ichannel, ":", ec);
        responce(responce_stream, ichannel, ec);
    }
}

