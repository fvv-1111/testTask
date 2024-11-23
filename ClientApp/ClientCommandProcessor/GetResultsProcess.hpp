#pragma once

#include <QtCore/QObject>
#include <unordered_set>

namespace SocketClient
{

class GetResultsProcess: public QObject
{
    Q_OBJECT
public:
    GetResultsProcess();
    ~GetResultsProcess() {};

    void add_channel(int ichannel);
    void remove_channel(int ichannel);

    void set_repeating_interval(std::chrono::milliseconds s);

private slots:
    void execute_read();

private:
    void start_timer();

    std::unordered_set<int> m_channels;
    bool m_is_running = false;

    std::chrono::milliseconds m_interval = std::chrono::milliseconds(500);
};

}