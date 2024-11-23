#pragma once

#include "../../Utils/ChannelEnums.hpp"
#include "../MVC/DataModel/DataModel.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <type_traits>

namespace SocketClient
{

namespace ClientCommands
{

class ICommand
{
public:
    virtual ~ICommand() {};

    virtual std::string get_server_command_string() const = 0;
    virtual void process_responce(const std::string&) = 0;
};


namespace CommandsUtils
{

inline void print(std::stringstream& ss, int ichannel)
{
    ss << " channel" << ichannel;
}

inline void print(std::stringstream& ss, const std::pair<int,SocketApp::ChannelRange>& data)
{
    ss << " channel" << data.first << ":" << SocketApp::to_string(data.second);
}

}

template <typename T, class TCommand>
class BaseCommand: public ICommand
{
public:
    void add(const T& arg)
    {
        m_command_args.push_back(arg);
    }

    template <typename T1, typename T2>
    std::enable_if_t<std::is_same_v<std::pair<T1,T2>,T>> add(T1 t1, T2 t2)
    {
        m_command_args.emplace_back(t1, t2);
    }

    std::string get_server_command_string() const override;

    void process_responce(const std::string& responce) override;

    virtual ~BaseCommand() {};

private:
    std::vector<T> m_command_args;
};

template <typename T, class TCommand>
std::string BaseCommand<T,TCommand>::get_server_command_string() const
{
    std::stringstream ss;
    ss << static_cast<const TCommand*>(this)->command_prefix();
    for (const T& arg: m_command_args)
    {
        CommandsUtils::print(ss, arg);
    }
    return ss.str();
}

template <typename T, class TCommand>
void BaseCommand<T,TCommand>::process_responce(const std::string& responce)
{
    MVC::Model::Data& data = MVC::Model::Data::singleton();
    static_cast<TCommand*>(this)->template execute<MVC::Model::Data>(responce, data);
}


}
}