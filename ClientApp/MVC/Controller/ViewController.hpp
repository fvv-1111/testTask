#pragma once

#include "../../../Utils/ChannelEnums.hpp"

#include <QtCore/QObject>
#include <memory>
#include <system_error>

namespace MVC
{

namespace Views
{
    class MainView;
}

class ViewController: public QObject
{
    Q_OBJECT
public:
    ViewController();
    virtual ~ViewController();

    void show();

private slots:
    void update_channel_range(int ichannel, SocketApp::ChannelRange r, std::error_code code);
    void update_channel_value_and_status(int ichannel, float v, SocketApp::ChannelStatus s, std::error_code code);
    void update_channel_start_stop_status(int ichannel, bool is_stared, std::error_code code);

    void change_range(int ichannel, SocketApp::ChannelRange r);
    void try_start_measure(int ichannel);
    void try_stop_measure(int ichannel);

    void server_operation_error(std::error_code ec);
    void server_connected();
private:
    std::unique_ptr<Views::MainView> m_main_view;
};

}