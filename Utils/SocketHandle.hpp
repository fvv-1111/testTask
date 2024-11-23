#pragma once

namespace SocketApp
{

namespace SocketHandleConst
{
constexpr const int invalid_socket_id = -1;
inline bool is_invalid(int i)
{
    return i == invalid_socket_id;
}
inline void set_invalid(int& i)
{
    i = invalid_socket_id;
}
}

class SocketHandle
{
public:

    SocketHandle();
    SocketHandle(int);
    SocketHandle(const SocketHandle&) = delete;
    void operator =(const SocketHandle&) = delete;
    SocketHandle(SocketHandle&&);
    void operator=(SocketHandle&&);

    ~SocketHandle();

    operator int()const;

    operator bool()const;

    void close();
private:
    int m_id;
};

}