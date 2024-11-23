#pragma once

#include "../../../Utils/ChannelEnums.hpp"
#include <QtCore/QObject>

namespace MVC
{
namespace Model
{

class ChannelData: public QObject
{
    Q_OBJECT
public:
    ChannelData(int ichannel);
    virtual ~ChannelData();

    void start_measure(std::error_code ec);
    void stop_measure(std::error_code ec);
    void set_value(float v);
    void set_value(std::error_code ec);
    void set_status(SocketApp::ChannelStatus s);
    void set_status(std::error_code ec);
    void set_range(SocketApp::ChannelRange r, std::error_code ec);
    void set_range(SocketApp::ChannelRange r);
    void set_range(std::error_code ec);

    bool is_measure_started()const;
    float value()const;
    SocketApp::ChannelRange range()const;
    SocketApp::ChannelStatus status()const;
    std::error_code error()const;


signals:
    void update_channel_range(int, SocketApp::ChannelRange, std::error_code);
    void update_channel_value_and_status(int, float, SocketApp::ChannelStatus, std::error_code);
    void update_channel_start_stop_status(int, bool, std::error_code);

private:
    int m_ichannel;
    bool m_measure_is_started = false;
    float m_value = 0;
    SocketApp::ChannelRange m_range = SocketApp::ChannelRange::Range0;
    SocketApp::ChannelStatus m_status = SocketApp::ChannelStatus::Idle;
    std::error_code m_error;
};
}
}