#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"

namespace ServerApp
{
class Connection;

namespace CommandProcessor
{

class GetResultCommand:public 
    Utils::CommandProcessor::BaseCommand<GetResultCommand,
                                         Utils::CommandProcessor::Command::GetResultRegex>
{
public:
    GetResultCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);
    friend class BaseCommand;
};

}

}