#pragma once

#include "StatusView.hpp"
#include "../../../Utils/ChannelEnums.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtCore/QPointer>

#include <string>
#include <system_error>

namespace MVC
{

namespace Views
{

class ChannelView: public QWidget
{
    Q_OBJECT
public:
    ChannelView(int ichannel, QWidget * parent);
    virtual ~ChannelView();


    ChannelStatusView& status_view();

    void update_range_view(SocketApp::ChannelRange r);
    void update_start_stop_view(bool is_start);
    void update_value(float v);
    void update_status(SocketApp::ChannelStatus s);

signals:
    void change_range(int, SocketApp::ChannelRange);
    void try_start_measure(int);
    void try_stop_measure(int);

public slots:
// event on control's change
    void measure_try_to_start_or_stop(Qt::CheckState s);
    void range_try_to_update(int index);

private:
    void create_view();

    int m_ichannel;
    ChannelStatusView m_status_view;

    QPointer<QComboBox> m_ranges;
    QPointer<QCheckBox> m_measure;
    QPointer<QLabel>    m_value;
    QPointer<QLabel>    m_channel_state;
};

}
}