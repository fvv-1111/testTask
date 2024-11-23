//
// Command "start_measure (channel[\d]+\s*(,)?)+"
//
#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"


namespace ServerApp
{

class Connection;

namespace CommandProcessor
{


class StartMeasureCommand: public 
    Utils::CommandProcessor::BaseCommand<StartMeasureCommand,
                                         Utils::CommandProcessor::Command::StartMeasureRegex>
{
public:
    StartMeasureCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);

    friend class BaseCommand;
};

}
}