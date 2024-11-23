#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{
namespace ClientCommands
{


class SetRangeCommand:public ClientCommands::BaseCommand<std::pair<int,SocketApp::ChannelRange>,SetRangeCommand>,
    Utils::CommandProcessor::BaseCommand<SetRangeCommand,
                                        Utils::CommandProcessor::Responce::SetRangeRegex>
{
public:
    SetRangeCommand() {};
    virtual ~SetRangeCommand() {};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class ClientCommands::BaseCommand<std::pair<int,SocketApp::ChannelRange>,SetRangeCommand>;
    friend class Utils::CommandProcessor::BaseCommand<SetRangeCommand,
                                                      Utils::CommandProcessor::Responce::SetRangeRegex>;
};
 
}
}