#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"

namespace ServerApp
{
class Connection;

namespace CommandProcessor
{

class StopMeasureCommand:public 
    Utils::CommandProcessor::BaseCommand<StopMeasureCommand,
                                         Utils::CommandProcessor::Command::StopMeasureRegex>
{
public:
    StopMeasureCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);

    friend class BaseCommand;
};

}

}