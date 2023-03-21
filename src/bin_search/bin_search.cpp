#include <fmt/ranges.h>
#include <iostream>
#include <range/v3/all.hpp>
#include <vector>

int main()
{
    std::vector<std::pair<int, char>> nums{{0, 'a'}, {0, 'b'}, {1, 'b'}};

    fmt::print("{}\n", nums);
    fmt::print("{}\n", std::get<int>(nums.front()));

    auto remove_duplicates = [](int &a, int &b) {
        a++;
        return a == b;
    };

    auto filtered_nums =
        nums | ranges::views::transform(&std::pair<int, char>::first) | ranges::views::unique(remove_duplicates);
    fmt::print("{}\n", filtered_nums);

    return 0;
}
