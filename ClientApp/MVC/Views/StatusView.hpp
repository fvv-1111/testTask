#pragma once

#include <QtWidgets/QLabel>
#include <QtCore/QPointer>

#include <string>
#include <system_error>

namespace MVC
{

namespace Views
{

class ChannelStatusView
{
public:
    ChannelStatusView();
    ~ChannelStatusView();

    QLabel* create(QWidget* parent);

    void measure_starting();
    void measure_in_progress();
    void measure_stopping();
    void measure_stopped();
    void update_range_starting();
    void range_updated();
    void error(const std::error_code& code);
    void ok();

private:
    QPointer<QLabel> m_lab;
};

}

}