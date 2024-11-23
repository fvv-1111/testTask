#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{
namespace ClientCommands
{


class GetRangeCommand:public ClientCommands::BaseCommand<int,GetRangeCommand>,
Utils::CommandProcessor::BaseCommand<GetRangeCommand, 
                                     Utils::CommandProcessor::Responce::GetRangeRegex>
{
public:
    GetRangeCommand() {};
    virtual ~GetRangeCommand(){};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class Utils::CommandProcessor::BaseCommand<GetRangeCommand, 
                                     Utils::CommandProcessor::Responce::GetRangeRegex>;
    friend class ClientCommands::BaseCommand<int,GetRangeCommand>;
};
 
}
}