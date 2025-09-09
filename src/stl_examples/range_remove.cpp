#include <algorithm>
#include <ranges>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <range/v3/all.hpp>

#define ICINFO(expr) SPDLOG_INFO("{}: {}", #expr, expr)

int main()
{
    std::vector<double> vec{1.2, 3.4, 3.5, 5.4};
    ICINFO(vec);

    // std range remove
    auto ret = std::ranges::remove(vec, 3.4);
    vec.erase(ret.begin(), ret.end());
    ICINFO(vec);

    // range-v3 remove
    auto it = ranges::remove(vec, 1.2);
    vec.erase(it, vec.end());
    ICINFO(vec);

    // enumerate
    for (auto &&[i, v] : vec | ranges::views::enumerate)
    {
        SPDLOG_INFO("i: {}, v: {}", i, v);
    }

    return 0;
}