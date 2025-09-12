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
    DerivedNode d1(1, 2.f, {{1, "one"}, {2, "two"}}, {1, 2, 3});

    spdlog::debug("d1: {}", d1);

    auto [data, in, out] = zpp::bits::data_in_out();
    OutputArchive oar{out};
    oar(std::string_view{typeid(d1).name()});
    d1.save(oar);

    DerivedNode d2;
    InputArchive iar{in};
    std::string type_name;
    iar(type_name);
    spdlog::debug("type_name: {}", type_name);
    d2.load(iar);

    spdlog::debug("d2: {}", d2);
}

void test_shared_ptr()
{
    std::shared_ptr<BaseNode> d1 = std::make_shared<DerivedNode>(
        1, 2.f, std::map<int, std::string>{{1, "one"}, {2, "two"}},
        std::deque{1, 2, 3});

    spdlog::debug("d1: {}", *d1);

    auto [data, in, out] = zpp::bits::data_in_out();
    OutputArchive oar{out};
    oar(d1);

    std::shared_ptr<BaseNode> d2;
    InputArchive iar{in};
    iar(d2);

    spdlog::debug("d2: {}", *d2);
}

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);

    // test_instance();
    test_shared_ptr();

    return 0;
}