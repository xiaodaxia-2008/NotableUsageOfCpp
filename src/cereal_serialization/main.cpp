#include "Derived.h"

#include <spdlog/spdlog.h>

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/std.h>

CEREAL_REGISTER_TYPE(Derived);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, Derived);

void test_instance()
{

    Derived d1(1, 2.f, {{1, "one"}, {2, "two"}}, {1, 2, 3});

    spdlog::debug("d1: {}", d1);

    std::stringstream ss;
    {
        cereal::JSONOutputArchive ar(ss);
        ar(d1);
    }

    spdlog::info("serialized result:\n{}", ss.str());

    Derived d2;
    {
        cereal::JSONInputArchive ar(ss);
        ar(d2);
    }
    spdlog::debug("d2: {}", d2);
}

void test_shared_ptr()
{
    std::shared_ptr<Base> d1 = std::make_shared<Derived>(
        1, 2.f, std::map<int, std::string>{{1, "one"}, {2, "two"}},
        std::deque{1, 2, 3});

    spdlog::debug("d1: {}", *d1);

    std::stringstream ss;
    {
        cereal::BinaryOutputArchive ar(ss);
        ar(d1);
    }

    spdlog::info("serialized result:\n{}", ss.str());

    std::shared_ptr<Base> d2;
    {
        cereal::BinaryInputArchive ar(ss);
        ar(d2);
    }
    spdlog::debug("d2: {}", *d2);
}

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);
    constexpr bool is_abstract = std::is_abstract_v<Derived>;

    test_shared_ptr();

    return 0;
}