#include "SocketHandle.hpp"
#include <utility>
#include <unistd.h>

using namespace SocketApp;

SocketHandle::SocketHandle()
:m_id(SocketHandleConst::invalid_socket_id)
{
    
}

SocketHandle::SocketHandle(int id)
:m_id(id)
{

}

SocketHandle::SocketHandle(SocketHandle&& h)
:m_id(h.m_id)
{
    SocketHandleConst::set_invalid(h.m_id);
}

void SocketHandle::operator=(SocketHandle&& h)
{
    if (this == &h)
        return;

    close();
    std::swap(m_id, h.m_id);
}

SocketHandle::~SocketHandle()
{
    close();
}

SocketHandle::operator int()const
{
    return m_id;
}

SocketHandle::operator bool()const
{
    return !SocketHandleConst::is_invalid(m_id);
}

void SocketHandle::close()
{
    if (*this)
    {
        int i = m_id;
        SocketHandleConst::set_invalid(m_id);
        ::close(i);
    }
}
