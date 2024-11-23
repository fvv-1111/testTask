#include "DataModel.hpp"

using namespace MVC::Model;

#ifndef NUM_CHANNELS
#define NUM_CHANNELS 8
#endif

constexpr const int nchannels = NUM_CHANNELS;

Data::Data()
{
    for (int i = 0; i < ::nchannels; ++i)
    {
        m_channels.emplace_back(new ChannelData(i));
    }
}

Data::~Data()
{

}

Data& Data::singleton()
{
    static Data data;
    return data;
}

int Data::nchannels()const
{
    return static_cast<int>(m_channels.size());
}

ChannelData& Data::operator[](int i)const
{
    return *m_channels[i].get();
}
