#include "MainView.hpp"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>

using namespace MVC::Views;

namespace {

int num_columns(int n)
{
    if (n > 16)
    {
        return 3;
    }
    else if (n > 4)
    {
        return 2;
    }
    return 1;
}

void to_row_col(int index, int ncols, int& irow, int& icol)
{
    icol = index % ncols;
    irow = index / ncols;
}

};

MainView::MainView()
{

}

void MainView::create(int nchannels)
{
    setWindowTitle("reader channel's data from server");

    auto * column = new QVBoxLayout(this);
    column->addStretch(1);

    auto * grid = new QGridLayout(this);

    m_channels.clear();
    m_channels.reserve(nchannels);
    int ncols_in_grid = ::num_columns(nchannels);
    for (int i = 0; i < nchannels; ++i)
    {
        int irow, icol;
        to_row_col(i, ncols_in_grid, irow, icol);
        auto * pc = new ChannelView(i, this);
        m_channels.emplace_back(pc);
        grid->addWidget(pc, irow, icol);
    }
    column->addLayout(grid);
    column->addStretch(1);
    setLayout(column);
}

QPointer<ChannelView> MainView::channel(int ichannel)
{
    return m_channels[ichannel];
}
