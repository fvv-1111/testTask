#include "GetStatusCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;

std::string GetStatusCommand::command_prefix() const
{
    return GetStatusRegex::prefix();
}

void GetStatusCommand::execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj)
{
    int ichannel = GetStatusRegex::channel_index(match_obj);
    std::variant<std::error_code, SocketApp::ChannelStatus> var = GetStatusRegex::get(match_obj);

    std::visit(
        Utils::visit_overloaded {
        [&data = obj, ichannel = ichannel] (SocketApp::ChannelStatus s) {
            data[ichannel].set_status(s);
        },
        [&data = obj, ichannel = ichannel] (std::error_code ec) {
            data[ichannel].set_status(ec);
        }
        },
        var
    );

}
