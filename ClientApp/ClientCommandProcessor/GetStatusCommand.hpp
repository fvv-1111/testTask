#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{

namespace ClientCommands
{


class GetStatusCommand:public ClientCommands::BaseCommand<int,GetStatusCommand>,
Utils::CommandProcessor::BaseCommand<GetStatusCommand, Utils::CommandProcessor::Responce::GetStatusRegex>
{
public:
    GetStatusCommand(){};
    virtual ~GetStatusCommand() {};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class ClientCommands::BaseCommand<int,GetStatusCommand>;
    friend class Utils::CommandProcessor::BaseCommand<GetStatusCommand, Utils::CommandProcessor::Responce::GetStatusRegex>;
};
 
}
}