#include "Logger.hpp"
#include <system_error>

using namespace Utils;

//============================================
Logger& Logger::singleton()
{
    static Logger logger;
    return logger;
}

//============================================
std::string Logger::last_error()
{
    std::error_code code = std::make_error_code(static_cast<std::errc>(errno));
    return code.message();
}

//============================================
Logger::~Logger()
{

}

//============================================
Logger::Logger()
{

}

