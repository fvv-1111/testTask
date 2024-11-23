#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"
#include "Command.hpp"

namespace SocketClient
{
namespace ClientCommands
{


class StopMeasureCommand:public ClientCommands::BaseCommand<int, StopMeasureCommand>,
    Utils::CommandProcessor::BaseCommand<StopMeasureCommand,
                                         Utils::CommandProcessor::Responce::StopMeasureRegex>
{
public:
    StopMeasureCommand() {};
    virtual ~StopMeasureCommand() {};

private:
    std::string command_prefix() const;

    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, MVC::Model::Data& obj);

    friend class ClientCommands::BaseCommand<int, StopMeasureCommand>;
    friend class Utils::CommandProcessor::BaseCommand<StopMeasureCommand,
                                         Utils::CommandProcessor::Responce::StopMeasureRegex>;
};
 
}
}