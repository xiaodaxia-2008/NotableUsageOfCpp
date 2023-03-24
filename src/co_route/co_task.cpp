#define IC(...) SPDLOG_INFO(#__VA_ARGS__ ": {}", __VA_ARGS__)

#include <concurrencpp/concurrencpp.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

concurrencpp::lazy_result<int> task1()
{
    SPDLOG_INFO("here");
    co_return 10;
}

concurrencpp::lazy_result<int> task2()
{
    SPDLOG_INFO("here");
    int n = co_await task1();
    SPDLOG_INFO("here");
    co_return n;
}

int main()
{
    SPDLOG_INFO("start");
    auto res2 = task2();
    SPDLOG_INFO("start task");
    res2.run();
    // IC(res2.resolve());
    SPDLOG_INFO("finish");
}