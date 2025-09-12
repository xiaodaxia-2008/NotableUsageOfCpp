#include <zpp_bits.h>

#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

#include <deque>
#include <iostream>
#include <string>

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

    auto z = std::make_shared<Person>(12, "Zen's");
    std::shared_ptr<Person> znull;
    out(znull);
    out(z);
    SPDLOG_INFO("z: {}", *z);

    // out(z_weak);
    std::shared_ptr<Person> z1, z2;
    in(z2);
    in(z1);
    SPDLOG_INFO("z1: {}", fmt::ptr(z1.get()));
    SPDLOG_INFO("z2: {}", fmt::ptr(z2.get()));
    SPDLOG_INFO("z1: {}", *z1);
    SPDLOG_INFO("z2: {}", fmt::ptr(z2.get()));

    return 0;
}