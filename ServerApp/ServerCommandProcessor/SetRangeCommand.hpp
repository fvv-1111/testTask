#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"


namespace ServerApp
{

class Connection;

namespace CommandProcessor
{

class SetRangeCommand: public 
    Utils::CommandProcessor::BaseCommand<SetRangeCommand,
                                         Utils::CommandProcessor::Command::SetRangeRegex>
{
public:
    SetRangeCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);

    friend class BaseCommand;
};


}
}