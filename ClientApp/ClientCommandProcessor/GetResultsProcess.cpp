#include "GetResultsProcess.hpp"


#include <QtCore/QTimer>
#include <chrono>
#include "CommandFactory.hpp"
#include "../ClientApp.hpp"

using namespace SocketClient;

GetResultsProcess::GetResultsProcess()
{

}

void GetResultsProcess::add_channel(int ichannel)
{
    m_channels.insert(ichannel);
    start_timer();
}

void GetResultsProcess::remove_channel(int ichannel)
{
    m_channels.erase(ichannel);
}

void GetResultsProcess::execute_read()
{
    m_is_running = false;
    if (m_channels.empty())
        return;
    
    ClientApp::singleton()->execute_command(
        CommandFactory::get_values_command(m_channels.begin(), m_channels.end())
    );
    start_timer();
}

void GetResultsProcess::set_repeating_interval(std::chrono::milliseconds s)
{
    m_interval = s;
}

void GetResultsProcess::start_timer()
{
    if (!m_is_running && !m_channels.empty())
    {
        m_is_running = true;
        QTimer::singleShot(m_interval, this, SLOT(execute_read()));
    }
}
