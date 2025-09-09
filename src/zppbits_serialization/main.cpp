#include "Derived.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/std.h>

void test_instance()
{

    Derived d1(1, 2.f, {{1, "one"}, {2, "two"}}, {1, 2, 3});

    spdlog::debug("d1: {}", d1);

    auto [data, in, out] = zpp::bits::data_in_out();

    out(d1);

    Derived d2;
    in(d2);

    spdlog::debug("d2: {}", d2);
}

// void test_shared_ptr()
// {
//     std::shared_ptr<Base> d1 = std::make_shared<Derived>(
//         1, 2.f, std::map<int, std::string>{{1, "one"}, {2, "two"}},
//         std::deque{1, 2, 3});

//     spdlog::debug("d1: {}", *d1);

//     std::stringstream ss;
//     {
//         cereal::BinaryOutputArchive ar(ss);
//         ar(d1);
//     }

//     spdlog::info("serialized result:\n{}", ss.str());

//     std::shared_ptr<Base> d2;
//     {
//         cereal::BinaryInputArchive ar(ss);
//         ar(d2);
//     }
//     spdlog::debug("d2: {}", *d2);
// }

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);

    test_instance();

    return 0;
}