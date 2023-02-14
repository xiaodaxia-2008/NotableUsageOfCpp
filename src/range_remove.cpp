#include <algorithm>
#include <ranges>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

#include <fmt/format.h>
#include <fmt/ranges.h>

#define ICINFO(expr) SPDLOG_INFO("{}: {}", #expr, expr)

int main()
{
    // remove a value in a range
    std::vector<double> vec{1.2, 3.4, 5.4};
    ICINFO(vec);
    auto ret = std::ranges::remove(vec, 3.4);
    vec.erase(ret.begin(), ret.end());
    ICINFO(vec);

    return 0;
}