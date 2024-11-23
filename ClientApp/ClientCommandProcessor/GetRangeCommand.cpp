#include "GetRangeCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;

std::string GetRangeCommand::command_prefix() const
{
    return GetRangeRegex::prefix();
}

void GetRangeCommand::execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj)
{
    int ichannel = GetRangeRegex::channel_index(match_obj);
    std::variant<SocketApp::ChannelRange, std::error_code> var = GetRangeRegex::get(match_obj);

    std::visit(
        Utils::visit_overloaded {
        [&data = obj, ichannel = ichannel] (SocketApp::ChannelRange r) {
            data[ichannel].set_range(r);
        },
        [&data = obj, ichannel = ichannel] (std::error_code ec) {
            data[ichannel].set_range(ec);
        }
        },
        var
    );



}
