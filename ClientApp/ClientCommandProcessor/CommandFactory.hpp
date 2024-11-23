#pragma once

#include "Command.hpp"
#include "GetResultCommand.hpp"

#include "../../Utils/ChannelEnums.hpp"

#include <memory>
#include <algorithm>

namespace SocketClient
{

class CommandFactory
{
public:
    static std::unique_ptr<ClientCommands::ICommand> get_range_command(int ichannel);
    static std::unique_ptr<ClientCommands::ICommand> get_range_all_channels_command();
    static std::unique_ptr<ClientCommands::ICommand> get_channel_status_command(int ichannel);
    static std::unique_ptr<ClientCommands::ICommand> set_range_command(int ichannel, SocketApp::ChannelRange r);
    static std::unique_ptr<ClientCommands::ICommand> start_measure_command(int ichannel);
    static std::unique_ptr<ClientCommands::ICommand> stop_measure_command(int ichannel);
    template<class Iterator>
    static std::unique_ptr<ClientCommands::ICommand> get_values_command(const Iterator& it_start, const Iterator& it_end)
    {
        std::unique_ptr<ClientCommands::GetResultCommand> pc = 
            std::make_unique<ClientCommands::GetResultCommand>();
        for (Iterator it = it_start; it != it_end; ++it)
            pc->add(*it);
        return pc;
    }
};


}
