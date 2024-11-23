#include "CommandFactory.hpp"
#include "GetStatusCommand.hpp"
#include "SetRangeCommand.hpp"
#include "GetRangeCommand.hpp"
#include "StartMeasureCommand.hpp"
#include "StopMeasureCommand.hpp"
#include "../MVC/DataModel/DataModel.hpp"

using namespace SocketClient;

std::unique_ptr<ClientCommands::ICommand> CommandFactory::get_range_all_channels_command()
{
    std::unique_ptr<ClientCommands::GetRangeCommand> pc = 
        std::make_unique<ClientCommands::GetRangeCommand>();
    for (int i = 0, n = static_cast<int>(MVC::Model::Data::singleton().nchannels()); i < n; ++i)
        pc->add(i);
    return pc;
}

std::unique_ptr<ClientCommands::ICommand> CommandFactory::get_range_command(int ichannel)
{
    std::unique_ptr<ClientCommands::GetRangeCommand> pc = 
        std::make_unique<ClientCommands::GetRangeCommand>();
    pc->add(ichannel);
    return pc;
}

std::unique_ptr<ClientCommands::ICommand> CommandFactory::set_range_command(
    int ichannel,
    SocketApp::ChannelRange r
)
{
    std::unique_ptr<ClientCommands::SetRangeCommand> pc = 
        std::make_unique<ClientCommands::SetRangeCommand>();
    pc->add(ichannel, r);
    return pc;
}

std::unique_ptr<ClientCommands::ICommand> CommandFactory::get_channel_status_command(int ichannel)
{
    std::unique_ptr<ClientCommands::GetStatusCommand> pc = 
        std::make_unique<ClientCommands::GetStatusCommand>();
    pc->add(ichannel);
    return pc;
}

std::unique_ptr<ClientCommands::ICommand> CommandFactory::start_measure_command(int ichannel)
{
    std::unique_ptr<ClientCommands::StartMeasureCommand> pc = 
        std::make_unique<ClientCommands::StartMeasureCommand>();
    pc->add(ichannel);
    return pc;
}

std::unique_ptr<ClientCommands::ICommand> CommandFactory::stop_measure_command(int ichannel)
{
    std::unique_ptr<ClientCommands::StopMeasureCommand> pc = 
        std::make_unique<ClientCommands::StopMeasureCommand>();
    pc->add(ichannel);
    return pc;
}
