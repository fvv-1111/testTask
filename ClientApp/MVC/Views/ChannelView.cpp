#include "ChannelView.hpp"

#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <sstream>

using namespace MVC::Views;

namespace
{
    std::string channel_title(int ichannel)
    {
        std::stringstream ss;
        ss << "channel " << ichannel << " :";
        return ss.str();
    }

    void init_range_combobox(QComboBox& box)
    {
        for (int i = 0; i < SocketApp::size_int<SocketApp::ChannelRange>(); ++i)
        {
            box.addItem(SocketApp::to_string(static_cast<SocketApp::ChannelRange>(i)).c_str());
        }
    }
}

ChannelView::ChannelView(int ichannel, QWidget * parent)
:QWidget(parent),
m_ichannel(ichannel)
{
    create_view();
}

ChannelView::~ChannelView()
{

}

void ChannelView::create_view()
{
    auto * cols = new QVBoxLayout(this);
    QFrame* pframe = new QFrame(this);
    pframe->setFrameStyle(QFrame::Box);
    cols->addWidget(pframe);

    auto * cols1 = new QVBoxLayout(pframe);

    auto * row1 = new QHBoxLayout();
    auto * lab1 = new QLabel(channel_title(m_ichannel).c_str(), this);

    m_ranges = new QComboBox(this);
    init_range_combobox(*m_ranges);
    QObject::connect(m_ranges.get(), SIGNAL (activated(int)), this, SLOT (range_try_to_update(int)));

    QLabel* start_measure_lab = new QLabel("start/stop measure", this);
    m_measure = new QCheckBox(this);

    QObject::connect(m_measure.get(), SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(measure_try_to_start_or_stop(Qt::CheckState)));

    row1->addWidget(lab1);
    row1->addStretch(1);
    row1->addWidget(m_ranges.get());
    row1->addSpacing(4);
    row1->addWidget(start_measure_lab);
    row1->addWidget(m_measure.get());

    auto * row2 = new QHBoxLayout();

    auto * msg = m_status_view.create(this);
    m_value = new QLabel("value : 0", this);
    m_channel_state = new QLabel("state : idle", this);

    row2->addWidget(m_value.get());
    row2->addSpacing(1);
    row2->addWidget(m_channel_state.get());
    row2->addStretch(1);
    row2->addWidget(msg);

    cols1->addLayout(row1);
    cols1->addLayout(row2);

    pframe->setLayout(cols1);
    setLayout(cols);
}

ChannelStatusView& ChannelView::status_view()
{
    return m_status_view;
}

void ChannelView::update_range_view(SocketApp::ChannelRange r)
{
    if (m_ranges)
        m_ranges->setCurrentIndex(static_cast<int>(r));
}

void ChannelView::update_start_stop_view(bool is_start)
{
    if (m_measure)
        m_measure->setCheckState(is_start ? Qt::CheckState::Checked: Qt::CheckState::Unchecked);
}

void ChannelView::update_value(float v)
{
    if (m_value)
    {
        std::stringstream ss;
        ss << "value : " << v;
        m_value->setText(ss.str().c_str());
    }
}

void ChannelView::update_status(SocketApp::ChannelStatus s)
{
    if (m_channel_state)
    {
        std::stringstream ss;
        ss << "status : " << SocketApp::to_string(s);
        m_channel_state->setText(ss.str().c_str());
    }
}

void ChannelView::measure_try_to_start_or_stop(Qt::CheckState s)
{
    if (s == Qt::Checked)
        emit try_start_measure(m_ichannel);
    else
        emit try_stop_measure(m_ichannel);
}

void ChannelView::range_try_to_update(int selected_range_index)
{
    SocketApp::ChannelRange r = static_cast<SocketApp::ChannelRange>(selected_range_index);
    emit change_range(m_ichannel, r);
}
