#include "SetRangeCommand.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../Connection.hpp"
#include "../../Utils/ChannelEnums.hpp"
#include "../../Utils/ErrorCodeDefinition.hpp"
#include "../../Utils/Logger.hpp"

using namespace ServerApp::CommandProcessor;
using namespace Utils::CommandProcessor::Command;
using namespace Utils;


void SetRangeCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    ServerApp::Connection& conn_obj)
{
    int ichannel = SetRangeRegex::channel_index(match_obj);
    SocketApp::ChannelRange range = SetRangeRegex::range(match_obj);
    SocketApp::ChannelRange set_range;
    std::error_code ec;
    ec = SocketApp::Channels::ChannelsSet::singleton().set_range(ichannel, range, conn_obj.connection_id());
    SocketApp::Channels::ChannelsSet::singleton().range(ichannel, set_range);

    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " set range command - channel", ichannel, ":", SocketApp::to_string(set_range));
        responce(responce_stream, ichannel, set_range);
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " set range command - error in channel", ichannel, ":", ec);
        responce(responce_stream, ichannel, ec, ":", set_range);
    }
}

