#include "CoObj.h"
#include <QApplication>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <coroutine>

tl::generator<int> F()
{
    SPDLOG_INFO("init coroutine");
    int i = 0;
    SPDLOG_INFO("co_yield i: {}", i);
    co_yield i;
    SPDLOG_INFO("after co_yield i: {}", i);
    ++i;
    SPDLOG_INFO("co_yield i: {}", i);
    co_yield i;
    SPDLOG_INFO("after co_yield i: {}", i);
    co_return;
}

CoObj::CoObj()
{
    SPDLOG_INFO("get coroutine");
    gen = F();
    SPDLOG_INFO("get coroutine begin");
    it = gen.begin();
    SPDLOG_INFO("after get coroutine begin");

    timer_id = startTimer(std::chrono::milliseconds(1000));
}

void CoObj::timerEvent(QTimerEvent *evet)
{
    SPDLOG_INFO("timer event");
    if (it != gen.end())
    {
        SPDLOG_INFO("*it: {}", *it);
        ++it;
        if (it != gen.end())
        {
            SPDLOG_INFO("*it: {}", *it);
        }
        else
        {
            SPDLOG_INFO("reached to end!");
            killTimer(timer_id);
            qApp->exit();
        }
    }
}