#include "Archive.h"
#include "Derived.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/ranges.h>

void test_instance()
{
    DerivedNode d1("A", {1, 2, 3});

    spdlog::info("d1: {}", d1);

    std::vector<std::byte> data;
    OutArchive ar(data);
    d1.serialize(ar);

    spdlog::info("serialized result:\n{}", data);

    InArchive iar(data);
    DerivedNode d2;
    d2.serialize(iar);
    spdlog::info("d2: {}", d2);
}

// void test_shared_ptr()
// {
//     std::shared_ptr<BaseNode> node1 = std::make_shared<BaseNode>("Node1");
//     std::shared_ptr<BaseNode> node2 =
//         std::make_shared<DerivedNode>("Node2", std::array<float, 3>{2, 2,
//         2});
//     node1->AddChild(node2);

//     SPDLOG_INFO("node1: {}", *node1);

//     std::stringstream ss;
//     {
//         cereal::BinaryOutputArchive ar(ss);
//         ar(node1);
//     }

//     spdlog::info("serialized result:\n{}", ss.str());
//     node1.reset();
//     {
//         cereal::BinaryInputArchive ar(ss);
//         ar(node1);
//     }
//     SPDLOG_INFO("node1: {}", *node1);
// }

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);

    test_instance();
    // test_shared_ptr();

    return 0;
}