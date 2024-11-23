#pragma once

#include <QtCore/QEvent>
#include "Command.hpp"
#include <string>
#include <system_error>

namespace SocketClient
{
namespace ClientCommands
{

class ResponceEvent: public QEvent
{
public:
    static QEvent::Type event_type();

    ResponceEvent(std::error_code ec);
    
    ResponceEvent(std::unique_ptr<ICommand>&& pcom, const std::string& responce);

    virtual ~ResponceEvent() {};

    ResponceEvent* clone() const override;

    std::error_code error_code()const;

    void execute_responce();
private:
    ResponceEvent(const ResponceEvent& );

    std::shared_ptr<ICommand> m_command;
    std::string m_responce;
    std::error_code m_ec;
};

}
}