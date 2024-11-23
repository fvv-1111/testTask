#pragma once 

#include <system_error>
#include <string>
#include <tuple>

#include "SocketHandle.hpp"

namespace Utils
{

struct SocketMessage
{
    // return value - <error_code,read string, true if connection was closed>
    static std::tuple<std::error_code, std::string, bool> read(const SocketApp::SocketHandle& h);
    
    static std::error_code write(const SocketApp::SocketHandle& h, const std::string& msg);
};

}