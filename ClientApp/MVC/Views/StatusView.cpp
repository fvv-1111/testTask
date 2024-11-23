#include "StatusView.hpp"

using namespace MVC::Views;


ChannelStatusView::ChannelStatusView()
{

}

ChannelStatusView::~ChannelStatusView()
{

}

QLabel* ChannelStatusView::create(QWidget* parent)
{
    m_lab = new QLabel("   ", parent);
    return m_lab.get();
}

void ChannelStatusView::measure_starting()
{
    if (m_lab)
        m_lab->setText("Measure starting...");
}

void ChannelStatusView::measure_in_progress()
{
    if (m_lab)
        m_lab->setText("Measure started");
}

void ChannelStatusView::measure_stopping()
{
    if (m_lab)
        m_lab->setText("Measure stopping...");
}

void ChannelStatusView::measure_stopped()
{
    if (m_lab)
        m_lab->setText("Measure stopped");
}

void ChannelStatusView::update_range_starting()
{
    if (m_lab)
        m_lab->setText("Range updating...");
}

void ChannelStatusView::range_updated()
{
    if (m_lab)
        m_lab->setText("Range updated");
}

void ChannelStatusView::error(const std::error_code& code)
{
    if (m_lab)
        m_lab->setText(code.message().c_str());
}

void ChannelStatusView::ok()
{
    if (m_lab)
        m_lab->setText(" ");
}
