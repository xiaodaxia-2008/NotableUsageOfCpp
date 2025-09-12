#include "Derived.h"

#include <spdlog/spdlog.h>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/std.h>

CEREAL_REGISTER_TYPE(DerivedNode);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseNode, DerivedNode);

void test_instance()
{

    DerivedNode d1("A", {1, 2, 3});

    spdlog::debug("d1: {}", d1);

    std::stringstream ss;
    {
        cereal::JSONOutputArchive ar(ss);
        ar(d1);
    }

    spdlog::info("serialized result:\n{}", ss.str());

    DerivedNode d2;
    {
        cereal::JSONInputArchive ar(ss);
        ar(d2);
    }
    spdlog::debug("d2: {}", d2);
}

void test_shared_ptr()
{
    std::shared_ptr<BaseNode> node1 = std::make_shared<BaseNode>("Node1");
    std::shared_ptr<BaseNode> node2 =
        std::make_shared<DerivedNode>("Node2", std::array<float, 3>{2, 2, 2});
    node1->AddChild(node2);

    SPDLOG_INFO("node1: {}", *node1);

    std::stringstream ss;
    {
        cereal::BinaryOutputArchive ar(ss);
        ar(node1);
    }

    spdlog::info("serialized result:\n{}", ss.str());
    node1.reset();
    {
        cereal::BinaryInputArchive ar(ss);
        ar(node1);
    }
    SPDLOG_INFO("node1: {}", *node1);
}

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);
    constexpr bool is_abstract = std::is_abstract_v<DerivedNode>;

    test_shared_ptr();

    return 0;
}