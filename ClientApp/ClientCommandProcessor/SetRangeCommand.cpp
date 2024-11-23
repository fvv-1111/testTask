#include "SetRangeCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;

std::string SetRangeCommand::command_prefix() const
{
    return SetRangeRegex::prefix();
}


void SetRangeCommand::execute_impl(
    const std::smatch& match_obj, 
    std::stringstream& responce_stream, 
    MVC::Model::Data& obj)
{
    int ichannel = SetRangeRegex::channel_index(match_obj);
    std::error_code err = SetRangeRegex::get_error_code(match_obj);
    SocketApp::ChannelRange r = SetRangeRegex::get_range(match_obj);
    obj[ichannel].set_range(r, err);
}
