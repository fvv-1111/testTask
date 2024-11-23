#pragma once

#include "ChannelData.hpp"
#include <vector>
#include <memory>

namespace MVC
{

namespace Model
{



class Data
{
public:
    virtual ~Data();

    static Data& singleton();

    int nchannels()const;

    ChannelData& operator[](int i)const;

private:
    Data();

    std::vector<std::unique_ptr<ChannelData>> m_channels;

    // no copy
    Data(const Data&) = delete;
    Data(Data&&) = delete;
    void operator=(const Data&) = delete;
    void operator=(Data&&) = delete;
};



}
}