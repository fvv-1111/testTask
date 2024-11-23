#include "StartMeasureCommand.hpp"
#include "../ChannelSimulator/ChannelsSet.hpp"
#include "../Connection.hpp"
#include "../../Utils/Logger.hpp"


using namespace ServerApp::CommandProcessor;
using namespace Utils::CommandProcessor::Command;
using namespace Utils;


void StartMeasureCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    Connection& conn_obj)
{
    int ichannel = StartMeasureRegex::channel_index(match_obj);
    std::error_code ec = SocketApp::Channels::ChannelsSet::singleton().start_measure(ichannel, conn_obj.connection_id());
    if (!ec)
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " start measure command - channel", ichannel, ":measure started");
        responce(responce_stream, ichannel, "ok");
    }
    else
    {
        Logger::singleton()("Connection ", conn_obj.connection_id(), 
            " start measure command - channel", ichannel, ":fail", ec);
        responce(responce_stream, ichannel, ec);
    }
}


