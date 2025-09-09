#include <zpp_bits.h>

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

#include <iostream>
#include <string>
#include <deque>

struct Person {
    int age{13};
    std::string name{"Zen"};
};

template <>
struct fmt::formatter<Person> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    auto format(const Person &p, format_context &ctx) const
    {
        return format_to(ctx.out(), "Person{{age: {}, name: {}}}", p.age,
                         p.name);
    }
};

int main()
{
    auto [data, in, out] = zpp::bits::data_in_out();

    Person person{.age = 14, .name = "Zen Shawn"};

    SPDLOG_INFO("person: {}", person);

    out(person).or_throw();

    Person person1;

    in(person1).or_throw();
    SPDLOG_INFO("person1: {}", person1);

    std::deque<int> queue{1, 2, 3};

    SPDLOG_INFO("queue: {}", queue);

    out(queue);

    std::deque<int> queue1;
    in(queue1);
    SPDLOG_INFO("queue1: {}", queue1);

    return 0;
}