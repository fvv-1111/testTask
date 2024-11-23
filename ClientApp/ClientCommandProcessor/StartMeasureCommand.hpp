#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{
namespace ClientCommands
{


class StartMeasureCommand:public ClientCommands::BaseCommand<int, StartMeasureCommand>,
    Utils::CommandProcessor::BaseCommand<StartMeasureCommand,
                                         Utils::CommandProcessor::Responce::StartMeasureRegex>
{
public:
    StartMeasureCommand() {};
    virtual ~StartMeasureCommand() {};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class ClientCommands::BaseCommand<int, StartMeasureCommand>;
    friend class Utils::CommandProcessor::BaseCommand<StartMeasureCommand,
                                         Utils::CommandProcessor::Responce::StartMeasureRegex>;
};
 
}
}