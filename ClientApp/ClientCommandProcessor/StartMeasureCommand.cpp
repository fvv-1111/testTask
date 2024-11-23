#include "StartMeasureCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;


std::string StartMeasureCommand::command_prefix() const
{
    return StartMeasureRegex::prefix();
}

void StartMeasureCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    MVC::Model::Data& data)
{
    int ichannel = StartMeasureRegex::channel_index(match_obj);
    std::error_code ec = StartMeasureRegex::get_error_code(match_obj);
    data[ichannel].start_measure(ec);
}
