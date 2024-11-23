#pragma once

#include "../../Utils/CommandProcessor/BaseCommand.hpp"
#include "../../Utils/CommandProcessor/RegexFactory.hpp"

namespace ServerApp
{
class Connection;

namespace CommandProcessor
{

class GetStatusCommand:public 
    Utils::CommandProcessor::BaseCommand<GetStatusCommand,
                                         Utils::CommandProcessor::Command::GetStatusRegex>
{
public:
    GetStatusCommand() {};

private:
    void execute_impl(const std::smatch& match_obj, std::stringstream& responce_stream, Connection& conn_obj);

    friend class BaseCommand;
};

}

}