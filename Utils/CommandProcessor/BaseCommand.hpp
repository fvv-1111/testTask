#pragma once 

#include "../ErrorCodeDefinition.hpp"
#include "../ChannelEnums.hpp"

#include <regex>
#include <sstream>


namespace Utils
{

namespace CommandProcessor
{

template<class Command, class RegexFactory>
class BaseCommand
{
public:
    bool is_mycommand(const std::string& com) const
    {
        return std::regex_match(com, m_match_regex);
    };

    template<typename T>
    std::string execute(const std::string &str, T& obj)
    {
        std::stringstream responce_stream;
        for (std::sregex_iterator it(str.begin(), str.end(), m_search_regex); it != std::sregex_iterator{}; ++it)
        {
            static_cast<Command*>(this)->execute_impl(*it, responce_stream, obj);
        }

        return responce_stream.str();
    }

    virtual ~BaseCommand() {};

protected:
    BaseCommand()
    :m_match_regex(RegexFactory::match_regex_string(), std::regex_constants::icase),
    m_search_regex(RegexFactory::item_regex_string(), std::regex_constants::icase)
    {

    }


    template<typename...T>
    static void responce(std::stringstream& ss, int ichannel, T&&... args)
    {
        ss << "channel" << ichannel << ":";
        responce_impl(ss, std::forward<T>(args)...);
    }


private:
    template<typename T>
    static void responce_impl1(std::stringstream& ss, T t)
    {
        ss << t;
    }

    static void responce_impl1(std::stringstream& ss, std::error_code ec)
    {
        SocketApp::to_stream(ss, ec);
    }

    static void responce_impl1(std::stringstream& ss, SocketApp::ChannelRange t)
    {
        ss << SocketApp::to_string(t);
    }

    static void responce_impl1(std::stringstream& ss, SocketApp::ChannelStatus t)
    {
        ss << SocketApp::to_string(t);
    }

    template<typename T1, typename...TT>
    static void responce_impl(std::stringstream& ss, T1&& t, TT&&...tt)
    {
        responce_impl1(ss, std::forward<T1>(t));
        if constexpr (sizeof...(TT) > 0)
            responce_impl(ss, std::forward<TT>(tt)...);
        ss << ' ';
    }

    std::regex m_match_regex;
    std::regex m_search_regex;
};

}
}