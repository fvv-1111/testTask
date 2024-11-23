#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QPointer>

#include "ChannelView.hpp"
#include <vector>


namespace MVC
{

namespace Views
{


class MainView: public QWidget
{
public:
    MainView();

    void create(int nchannels);

    QPointer<ChannelView> channel(int ichannel);

private:
    std::vector<QPointer<ChannelView>> m_channels;
};

}
}