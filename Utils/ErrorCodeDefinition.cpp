#include "ErrorCodeDefinition.hpp"
#include <sstream>

using namespace SocketApp;

//==================================================================
const char* SocketAppErrorCategory::name() const noexcept
{
    return "SocketAppErrors";
}

//==================================================================
std::string SocketAppErrorCategory::message(int val) const
{
    std::stringstream ss;
    ss << name() << ":";
    switch (static_cast<AppErrors>(val))
    {
        case AppErrors::UnrecognizedCommand :
            ss << "unrecognized command.";
            break;
        case AppErrors::IncorrectChannelIndex :
            ss << "incorrect channel index";
            break;
        case AppErrors::IncorrectRangeIndex :
            ss << "incorrect range value";
            break;
        case AppErrors::RangeWasLockedAnotherUser :
            ss << "range value for this channel was set by another user";
            break;
        case AppErrors::ChannelNotRunning :
            ss << "this channel is not started to produce measure";
            break;
        case AppErrors::RangeLockedByChannelStarted :
            ss << "range of this channel is locked because the channel is started to produce measure";
            break;
        case AppErrors::ChannelErrorStatus :
            ss << "channel is in Error state. Operation is impossible.";
            break;
        case AppErrors::ChannelBusyStatus:
            ss << "channel is in Busy state. Operation is impossible.";
            break;
        case AppErrors::ChannelIdleStatus:
            ss << "channel is in Idle state. Operation is impossible.";
            break;
        case AppErrors::UnknowCategoryNameError:
            ss << "unrecorgnized error.";
            break;
        case AppErrors::ConnectionWithServerBroken:
            ss << "connection with server is broken.";
            break;
        default:
            break;
    }
    return ss.str();
}

//==================================================================
SocketAppErrorCategory& SocketAppErrorCategory::singleton()
{
    static SocketAppErrorCategory val;
    return val;
}

namespace SocketApp
{
    
//==================================================================
std::error_code create_error_code(const std::string& category_name, int code)
{
    const std::error_category* arr[] = {
        std::addressof(SocketAppErrorCategory::singleton()),
        std::addressof(std::system_category()),
        std::addressof(std::iostream_category()),
        std::addressof(std::generic_category())
    };
    for (const std::error_category* p : arr)
    {
        if (Utils::compare_case_insensitive(std::string_view(p->name()), std::string_view(category_name)))
        {
            return std::error_code(code, *p);
        }
    }
    return std::make_error_code(AppErrors::UnknowCategoryNameError);
}

}