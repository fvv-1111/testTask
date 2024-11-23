#pragma once

#include <system_error>
#include <sstream>

namespace SocketApp
{
    enum class AppErrors: int
    {
        UnrecognizedCommand,
        IncorrectChannelIndex,
        IncorrectRangeIndex,

        // another user was set range value for a channel, then you try to change it.
        RangeWasLockedAnotherUser,
        
        // you try to get value from a channel which is not started to measure
        ChannelNotRunning,

        // range is locked to change because channel was start to measure
        RangeLockedByChannelStarted,

        // try to get value when channel is not in "start_measure" status
        ChannelErrorStatus,
        ChannelBusyStatus,
        ChannelIdleStatus,

        // attempt to read error code with unrecorgnized category name
        UnknowCategoryNameError,

        ConnectionWithServerBroken

    };

    class SocketAppErrorCategory: public std::error_category
    {
    public:
        const char* name() const noexcept override;
        std::string message(int val) const override;

        static SocketAppErrorCategory& singleton();
    };

    inline void to_stream(std::stringstream& ss, std::error_code ec)
    {
        ss << "fail[" << ec.category().name() << ":" << ec.value() << "]";
    };

    inline std::string to_responce_string(std::error_code ec)
    {
        std::stringstream ss;
        to_stream(ss, ec);
        return ss.str();
    };

    std::error_code create_error_code(const std::string& category_name, int code);
}

namespace Utils
{
inline bool compare_case_insensitive(wchar_t c1, wchar_t c2)
{
    return std::towlower(c1) == std::towlower(c2);
}

inline bool compare_case_insensitive(char c1, char c2)
{
    return std::tolower(c1) == std::tolower(c2);
}

template<class Str>
inline bool compare_case_insensitive(const Str& s1, const Str& s2)
{
    if (s1.length() != s2.length())
        return false;

    return std::equal(s1.begin(), s1.end(), s2.begin(), [](auto c1, auto c2) {
            return compare_case_insensitive(c1, c2);
    });
}
}

namespace std
{
    template<>
    struct is_error_code_enum<SocketApp::AppErrors>: true_type {};

    inline error_code make_error_code(SocketApp::AppErrors err) noexcept
    {
        return error_code(static_cast<int>(err), SocketApp::SocketAppErrorCategory::singleton());
    }

}