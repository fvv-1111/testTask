//
// Simple logger(it use std::cout for print messages on console)
//
#pragma once

#include <mutex>
#include <string>
#include <iostream>

namespace Utils
{

class Logger
{
public:
    static Logger& singleton();

    template <typename...Args>
    void operator()(Args...args);

    static std::string last_error();

    ~Logger();

private:
    Logger();

    // no copy!!!
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    void operator=(Logger&&) = delete;


private: // variables
    std::mutex m_mutex;// for excluding 
};

template <typename...Args>
void Logger::operator()(Args...args)
{
    std::lock_guard lock(m_mutex);
    (std::cout << ... << args) << std::endl;
}

}