#define IC(...) SPDLOG_INFO(#__VA_ARGS__ ": {}", __VA_ARGS__)

// #include <cppcoro/generator.hpp>
#include <spdlog/spdlog.h>

#include <generator.hpp>

template <typename T> using generator = tl::generator<T>;

struct BaseNode
{
    BaseNode()
    {
        SPDLOG_INFO("construct base: {}", fmt::ptr(this));
    }
    ~BaseNode()
    {
        SPDLOG_INFO("destruct base: {}", fmt::ptr(this));
    }
};

generator<int> fibonacci()
{
    SPDLOG_INFO("here");
    int a = 0, b = 1;
    SPDLOG_INFO("here");
    BaseNode base;
    SPDLOG_INFO("here");
    while (true)
    {
        if (b >= 3)
        {
            throw 1;
        }

        if (b >= 6)
        {
            throw std::runtime_error("invalid");
        }
        SPDLOG_INFO("here");
        co_yield b;
        SPDLOG_INFO("here");
        auto tmp = a;
        a = b;
        b += tmp;
    }
}

generator<int> wrapper(int n)
{
    for (auto &&i : fibonacci())
    {
        if (i >= n)
            co_return;
        co_yield i;
    }
}

int main()
{
    auto w = wrapper(10);
    try
    {
        for (auto &&i : w)
        {
            IC(i);
        }
    }
    catch (int &e)
    {
        SPDLOG_ERROR("Exception: {}", e);
    }
    catch (std::exception &e)
    {
        SPDLOG_ERROR("{}", e.what());
    }
    return 0;
}