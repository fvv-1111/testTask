#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{

namespace ClientCommands
{


class GetResultCommand:public ClientCommands::BaseCommand<int,GetResultCommand>,
Utils::CommandProcessor::BaseCommand<GetResultCommand, 
                                     Utils::CommandProcessor::Responce::GetResultRegex>
{
public:
    GetResultCommand() {};
    virtual ~GetResultCommand() {};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class Utils::CommandProcessor::BaseCommand<GetResultCommand, 
                                                      Utils::CommandProcessor::Responce::GetResultRegex>;
    friend class ClientCommands::BaseCommand<int,GetResultCommand>;
};
 
}
}