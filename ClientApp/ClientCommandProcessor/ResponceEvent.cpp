#include "ResponceEvent.hpp"

using namespace SocketClient::ClientCommands;

QEvent::Type ResponceEvent::event_type()
{
    static QEvent::Type type = static_cast<QEvent::Type>(QEvent::registerEventType());
    return type;
}

ResponceEvent::ResponceEvent(
    std::error_code ec
)
:QEvent(event_type()),
m_ec(ec)
{

}

ResponceEvent::ResponceEvent(
    std::unique_ptr<ICommand>&& pcom, 
    const std::string& responce
)
:QEvent(event_type()),
m_command(pcom.release()),
m_responce(responce)
{

}


ResponceEvent* ResponceEvent::clone() const
{
    return new ResponceEvent(*this);
}

void ResponceEvent::execute_responce()
{
    if (!m_ec)
    {
        m_command->process_responce(m_responce);
    }
}

std::error_code ResponceEvent::error_code()const
{
    return m_ec;
}


ResponceEvent::ResponceEvent(const ResponceEvent& event)
:QEvent(event),
m_command(event.m_command),
m_responce(event.m_responce),
m_ec(event.m_ec)
{

}
