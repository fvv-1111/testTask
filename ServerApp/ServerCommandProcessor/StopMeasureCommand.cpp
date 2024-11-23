#include "StopMeasureCommand.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../Connection.hpp"
#include "../../Utils/Logger.hpp"

using namespace ServerApp::CommandProcessor;
using namespace Utils;
using namespace Utils::CommandProcessor::Command;


void StopMeasureCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    Connection& conn_obj)
{
    int ichannel = StopMeasureRegex::channel_index(match_obj);
    std::error_code ec = SocketApp::Channels::ChannelsSet::singleton().stop_measure(ichannel, conn_obj.connection_id());
    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " stop measure command - channel", ichannel, ":measure stopped");
        responce(responce_stream, ichannel, "ok");
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " stop measure command - channel", ichannel, ":error stopped - ", ec);
        responce(responce_stream, ichannel, ec);
    }
}

