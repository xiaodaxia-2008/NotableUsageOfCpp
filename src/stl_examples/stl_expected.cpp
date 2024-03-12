#include <expected>
#include <string>
#include <print>

std::expected<std::string, int> f(int x)
{
    if (x < 0) {
        return std::unexpected(x);
    }
    return std::string("hello");
}

int main()
{
    auto res1 = f(1).and_then([](const std::string &s) {
        std::println("{}", s);
        return std::expected<void, int>{};
    });
    std::println("type of res: {}", typeid(res1).name());

    auto res2 =
        f(1).transform([](const std::string &s) { std::println("{}", s); });

    res2.transform_error([](int i) { std::println("error: {}", i); });
    std::println("type of res: {}", typeid(res2).name());
}