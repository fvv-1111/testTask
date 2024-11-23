#include "StopMeasureCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;


std::string StopMeasureCommand::command_prefix() const
{
    return StopMeasureRegex::prefix();
}


void StopMeasureCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    MVC::Model::Data& data)
{
    int ichannel = StopMeasureRegex::channel_index(match_obj);
    std::error_code ec = StopMeasureRegex::get_error_code(match_obj);
    data[ichannel].stop_measure(ec);
}
