#include "GetResultCommand.hpp"

using namespace SocketClient::ClientCommands;
using namespace Utils::CommandProcessor::Responce;

std::string GetResultCommand::command_prefix() const
{
    return GetResultRegex::prefix();
}

void GetResultCommand::execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj)
{
    int ichannel = GetResultRegex::channel_index(match_obj);
    std::variant<std::error_code, float> var = GetResultRegex::get(match_obj);

    std::visit(
        Utils::visit_overloaded {
        [&data = obj, ichannel = ichannel] (float t) {
            data[ichannel].set_value(t);
        },
        [&data = obj, ichannel = ichannel] (std::error_code ec) {
            data[ichannel].set_value(ec);
        }
        },
        var
    );



}
