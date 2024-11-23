#pragma once

#include "../ChannelEnums.hpp"
#include "../ErrorCodeDefinition.hpp"

#include <string>
#include <regex>
#include <optional>
#include <sstream>
#include <variant>
#include <cassert>


namespace Utils
{

template <typename... T>
struct visit_overloaded: public T...
{
    using T::operator()...;
};
template <typename... T>
visit_overloaded(T...) -> visit_overloaded<T...>;


namespace CommandProcessor
{

//
// When we search of items in string by regex in BaseCommand::execute(...), std::sregex_iterator returns 
// std::smatch object, it has operator[], indices for the operator is corresponding to group "(..)"
// in the regex string. 
// Classes in this file is for getting strings for building of regex and for getting index of the group
// for extracting substrings.

class ChannelIndex
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = "channel(\\d+)";
        return s;
    }

    static int number_items()
    {
        return 1;
    }

    static int get(int start_index, const std::smatch& m)
    {
        std::string s = m[start_index];
        return std::stoi(s);
    }

};

class RangeRegex
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = "range(\\d+)";
        return s;
    }

    // number of items added to std::smatch by get_regex_string()
    static int number_items()
    {
        return 1;
    }

    static SocketApp::ChannelRange get(int start_index, const std::smatch& m)
    {
        std::string s = m[start_index].str();
        int i = std::stoi(s);
        if (i < SocketApp::size_int<SocketApp::ChannelRange>())
        {
            return static_cast<SocketApp::ChannelRange>(i);
        }
        assert(0);
        return SocketApp::ChannelRange::Range0;
    }

};

class FailRegexString
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = "(fail\\[(\\w+):(\\d+)\\])";
        return s;
    }

    // number of items added to std::smatch by get_regex_string()
    static int number_items()
    {
        return 3;
    }

    static std::error_code get(int start_index, const std::smatch& m)
    {
        std::string cat_str = m[start_index + 1].str();
        std::string code_str = m[start_index + 2].str();
        if (!cat_str.empty() && !code_str.empty())
        {
            int code = std::stoi(code_str);
            return SocketApp::create_error_code(cat_str, code);
        }
        return {};
    }

};

class ChannelStatusRegex
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = channel_status_regex_string();
        return s;
    }

    // number of items added to std::smatch by get_regex_string()
    static int number_items()
    {
        return 1 + SocketApp::size<SocketApp::ChannelStatus>();
    }

    // check if the regex match object is stored data ChannelStatus and return the value in the case.
    // else return "empty".
    // start_index - index of group "(..)" in which get_regex_string() is stored.
    static SocketApp::ChannelStatus get(int start_index, const std::smatch& m)
    {
        std::string s = m[start_index].str();
        if (!s.empty())
        {
            for (int i = 1; i <= SocketApp::size_int<SocketApp::ChannelStatus>(); ++i)
            {
                if (!m[start_index + i].str().empty())
                    return static_cast<SocketApp::ChannelStatus>(i - 1);
            }
        }
        assert(0);
        return SocketApp::ChannelStatus::Error;
    }

private:
    static std::string channel_status_regex_string()
    {
        std::stringstream ss;
        ss << "(";
        for (int i = 0; i < SocketApp::size_int<SocketApp::ChannelStatus>(); ++i)
        {
            if (i)
                ss << "|";
            ss << "(" << SocketApp::to_string(static_cast<SocketApp::ChannelStatus>(i)) << ")";
        }
        ss << ")";
        return ss.str();
       
    }

};

class OKValue
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = "(ok)";
        return s;
    }

    // number of items added to std::smatch by get_regex_string()
    static int number_items()
    {
        return 1;
    }

};

class FloatValue
{
public:
    static const std::string& get_regex_string()
    {
        static std::string s = "([-+]?\\d+(\\.\\d*)?(e[-+]?\\d+)?)";
        return s;
    }

    // number of items added to std::smatch by get_regex_string()
    static int number_items()
    {
        return 3;
    }

    static float get(int start_index, const std::smatch& m)
    {
        std::string s = m[start_index].str();
        return std::stof(s);
    }
    
};


namespace Command
{

template<class T>
class CommandWithChannelIndex
{
public:
    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + T::prefix() + std::string("\\s+(") + 
            item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string();
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

};

class StartMeasureRegex: public CommandWithChannelIndex<StartMeasureRegex>
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "start_measure";
        return s;
    }
};

class StopMeasureRegex: public CommandWithChannelIndex<StopMeasureRegex>
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "stop_measure";
        return s;
    }
};

class GetStatusRegex: public CommandWithChannelIndex<GetStatusRegex>
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "get_status";
        return s;
    }
};


class GetResultRegex: public CommandWithChannelIndex<GetResultRegex>
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "get_result";
        return s;
    }
};

class GetRangeRegex: public CommandWithChannelIndex<GetRangeRegex>
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "get_range";
        return s;
    }

};

class SetRangeRegex
{
public:
    static const std::string& prefix()
    {
        static const std::string s = "set_range";
        return s;
    }

    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + prefix() + std::string("\\s+(") + 
            item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*" + RangeRegex::get_regex_string();
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static SocketApp::ChannelRange range(const std::smatch & m)
    {
        int i = 1 + ChannelIndex::number_items();
        return RangeRegex::get(i, m);
    }

};

} // namespace Command

namespace Responce
{

template<typename T>
class StartStopMeasureRegexBase
{
public:
    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + 
            std::string("\\s+(") + item_regex_string() + ")+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*(" + OKValue::get_regex_string() + "|" + 
          FailRegexString::get_regex_string() + ")\\s*";
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static std::error_code get_error_code(const std::smatch& m)
    {
        int iok = 2 + ChannelIndex::number_items();
        int ierr = iok + OKValue::number_items();
        return FailRegexString::get(ierr, m);     
    }
};

class StartMeasureRegex:public StartStopMeasureRegexBase<StartMeasureRegex>
// channel0:ok|error code
{
public:
    static const std::string& prefix()
    {
        return Command::StartMeasureRegex::prefix();
    }
};

class StopMeasureRegex:public StartStopMeasureRegexBase<StopMeasureRegex>
// channel0:ok|error code
{
public:
    static const std::string& prefix()
    {
        return Command::StopMeasureRegex::prefix();
    }
};

class GetRangeRegex 
// channel0:error|range value
{
public:
    static const std::string& prefix()
    {
        return Command::GetRangeRegex::prefix();
    }

    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + 
            std::string("\\s+(") + item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*(" + FailRegexString::get_regex_string() + "|(" +
           RangeRegex::get_regex_string() + "))";
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static std::variant<SocketApp::ChannelRange, std::error_code> get(const std::smatch& m)
    {
        int ierror = 2 + ChannelIndex::number_items();
        if (!m[ierror].str().empty())
            return FailRegexString::get(ierror, m);
        int irange = ierror + FailRegexString::number_items() + 1;
        return RangeRegex::get(irange, m);
    }

};

class SetRangeRegex 
//1 channel0:range value
//2 channel0:error:range value
{
public:
    static const std::string& prefix()
    {
        return Command::SetRangeRegex::prefix();
    }

    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + 
            std::string("\\s+(") + item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*(" + 
          "(" + FailRegexString::get_regex_string() + ":)?" + RangeRegex::get_regex_string() + ")";
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static std::error_code get_error_code(const std::smatch& m)
    {
        int i = ChannelIndex::number_items() + 3;
        std::string str = m[i].str();
        return FailRegexString::get(i, m);
    }

    static SocketApp::ChannelRange get_range(const std::smatch& m)
    {
        int i = ChannelIndex::number_items() + FailRegexString::number_items() + 3;
        return RangeRegex::get(i, m);
    }

};


class GetStatusRegex // channel0:(error|status)
{
public:
    static const std::string& prefix()
    {
        return Command::GetStatusRegex::prefix();
    }

    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + 
            std::string("\\s+(") + item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*(" + 
          FailRegexString::get_regex_string() + "|" + ChannelStatusRegex::get_regex_string() + ")";
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static std::variant<std::error_code, SocketApp::ChannelStatus> get(const std::smatch& m)
    {
        int i = ChannelIndex::number_items() + 2;
        std::string s1 = m[i].str();
        if (!s1.empty())
        {
            return FailRegexString::get(i, m);
        }
        int j = i + FailRegexString::number_items();
        return ChannelStatusRegex::get(j, m);
    }

};

class GetResultRegex // channel0:(error|fval)
{
public:
    static const std::string& prefix()
    {
        return Command::GetResultRegex::prefix();
    }

    static const std::string& match_regex_string()
    {
        static const std::string s = std::string("^\\s*") + 
            std::string("\\s+(") + item_regex_string() + "\\s*)+$";
        return s;
    }

    static const std::string& item_regex_string()
    {
        static const std::string s = ChannelIndex::get_regex_string() + 
          "\\s*:\\s*(" + 
          FailRegexString::get_regex_string() + "|" + FloatValue::get_regex_string() + ")";
        return s;
    }

    static int channel_index(const std::smatch & m)
    {
        return ChannelIndex::get(1, m);
    }

    static std::variant<std::error_code, float> get(const std::smatch& m)
    {
        int i = ChannelIndex::number_items() + 2;
        std::string s1 = m[i].str();
        if (!s1.empty())
        {
            return FailRegexString::get(i, m);
        }
        int j = i + FailRegexString::number_items();
        return FloatValue::get(j, m);
    }

};

} // namespace Responce

}
}