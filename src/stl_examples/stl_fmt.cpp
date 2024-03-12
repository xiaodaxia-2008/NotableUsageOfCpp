#include <format>
#include <print>
#include <vector>
#include <ranges>

int main()
{
    std::println("hello, world");

    std::vector vec{1.4, 2.4, 3.2, 4.7};
    std::println("vec: {}", vec);
}