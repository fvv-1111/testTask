#include "ViewController.hpp"
#include "../Views/MainView.hpp"
#include "../DataModel/DataModel.hpp"
#include "../../ClientApp.hpp"
#include "../../ClientCommandProcessor/CommandFactory.hpp"

#include <QtWidgets/QMessageBox>


using namespace MVC;

ViewController::ViewController()
{

}

ViewController::~ViewController()
{

}

void ViewController::show()
{
    Model::Data& data = Model::Data::singleton();
    int nchannels = data.nchannels();
    m_main_view = std::make_unique<Views::MainView>();
    m_main_view->create(nchannels);

    m_main_view->show();

    for (int i = 0; i < nchannels; ++i)
    {
        if (auto * p = m_main_view->channel(i).get())
        {
            QObject::connect(p, SIGNAL(change_range(int, SocketApp::ChannelRange)),
                             this, SLOT(change_range(int, SocketApp::ChannelRange)));
            QObject::connect(p, SIGNAL(try_start_measure(int)),
                             this, SLOT(try_start_measure(int)));
            QObject::connect(p, SIGNAL(try_stop_measure(int)),
                             this, SLOT(try_stop_measure(int)));
        }
        QObject::connect(
            &data[i], 
            SIGNAL(update_channel_range(int, SocketApp::ChannelRange, std::error_code)),
            this,
            SLOT(update_channel_range(int, SocketApp::ChannelRange, std::error_code))
        );
        QObject::connect(
            &data[i], 
            SIGNAL(update_channel_value_and_status(int, float, SocketApp::ChannelStatus, std::error_code)),
            this,
            SLOT(update_channel_value_and_status(int, float, SocketApp::ChannelStatus, std::error_code))
        );
        QObject::connect(
            &data[i], 
            SIGNAL(update_channel_start_stop_status(int, bool, std::error_code)),
            this,
            SLOT(update_channel_start_stop_status(int, bool, std::error_code))
        );
    }
    QObject::connect(
        SocketClient::ClientApp::singleton(),
        SIGNAL(server_operation_error(std::error_code)),
        this,
        SLOT(server_operation_error(std::error_code))
    );
    QObject::connect(
        SocketClient::ClientApp::singleton(),
        SIGNAL(server_connected()),
        this,
        SLOT(server_connected())
    );
}

void ViewController::update_channel_range(
    int ichannel, 
    SocketApp::ChannelRange r, 
    std::error_code code)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        if (code)
        {
            p->status_view().error(code);
        }
        else
        {
            p->status_view().range_updated();
        }
        p->update_range_view(r);
    }
}

void ViewController::update_channel_value_and_status(
    int ichannel, 
    float v, 
    SocketApp::ChannelStatus s, 
    std::error_code code
)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        if (code)
        {
            p->status_view().error(code);
        }
        else if (s == SocketApp::ChannelStatus::Measure)
        {
            p->status_view().measure_in_progress();
        }
        p->update_status(s);
        p->update_value(v);
    }
}

void ViewController::update_channel_start_stop_status(
    int ichannel, 
    bool is_stared, 
    std::error_code code
)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        if (code)
        {
            p->status_view().error(code);
        }
        else
        {
            if (is_stared)
            {
                p->status_view().measure_in_progress();
                SocketClient::ClientApp::singleton()->results_process().add_channel(ichannel);
            }
            else
            {
                p->status_view().measure_stopped();
            }
        }
        p->update_start_stop_view(is_stared);
    }
}

void ViewController::server_connected()
{
    SocketClient::ClientApp::singleton()->execute_command(
        SocketClient::CommandFactory::get_range_all_channels_command()
    );
}

void ViewController::server_operation_error(std::error_code ec)
{
    QMessageBox dlg;
    dlg.setWindowTitle("connection errors");
    std::stringstream ss;
    ss << "Error in read/write/connect to server : " << ec.message() << std::endl;
    ss << "This application will be closed.";
    dlg.setIcon(QMessageBox::Critical);
    dlg.setText(ss.str().c_str());
    dlg.setStandardButtons(QMessageBox::Close);
    dlg.exec();
    QApplication::postEvent(SocketClient::ClientApp::singleton(), new QEvent(QEvent::Quit));
}

void ViewController::change_range(int ichannel, SocketApp::ChannelRange r)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        p->status_view().update_range_starting();
        SocketClient::ClientApp::singleton()->execute_command(
            SocketClient::CommandFactory::set_range_command(ichannel, r)
        );
    }
}

void ViewController::try_start_measure(int ichannel)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        p->status_view().measure_starting();
        SocketClient::ClientApp::singleton()->execute_command(
            SocketClient::CommandFactory::start_measure_command(ichannel)
        );
    }

}

void ViewController::try_stop_measure(int ichannel)
{
    if (auto* p = m_main_view->channel(ichannel).get())
    {
        p->status_view().measure_stopping();
        SocketClient::ClientApp::singleton()->execute_command(
            SocketClient::CommandFactory::stop_measure_command(ichannel)
        );
        SocketClient::ClientApp::singleton()->results_process().remove_channel(ichannel);
        SocketClient::ClientApp::singleton()->execute_command(
            SocketClient::CommandFactory::get_channel_status_command(ichannel)
        );
    }

}
