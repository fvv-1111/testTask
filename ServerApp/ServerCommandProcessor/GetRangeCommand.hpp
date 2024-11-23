#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"


namespace ServerApp
{

class Connection;

namespace CommandProcessor
{

class GetRangeCommand: public 
    Utils::CommandProcessor::BaseCommand<GetRangeCommand,
                                         Utils::CommandProcessor::Command::GetRangeRegex>
{
public:
    GetRangeCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);

    friend class BaseCommand;
};


}
}