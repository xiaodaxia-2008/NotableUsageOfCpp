#include "Archive.h"
#include "Derived.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/ranges.h>

// void test_instance()
// {
//     DerivedNode d1("A", {1, 2, 3});

//     spdlog::info("d1: {}", d1);

//     OutArchive ar;
//     d1.serialize(ar);

//     auto data = ar.GetBuffer();

//     spdlog::info("serialized result:\n{}", data);

//     InArchive iar(data);
//     DerivedNode d2;
//     d2.serialize(iar);
//     spdlog::info("d2: {}", d2);
// }

void test_shared_ptr()
{
    std::shared_ptr<BaseNode> node1 =
        std::make_shared<DerivedNode>("Node1", std::array<float, 3>{2, 2, 2});
    std::shared_ptr<BaseNode> node2 = std::make_shared<BaseNode>("Node2");
    std::shared_ptr<BaseNode> node3 =
        std::make_shared<DerivedNode>("Node3", std::array<float, 3>{3, 3, 3});
    node1->AddChild(node2);
    node2->AddChild(node3);
    SPDLOG_INFO("node1: {}", *node1);
    SPDLOG_INFO("node2: {}", *node2);
    SPDLOG_INFO("node3: {}", *node3);

    REGISTER_CLASS(BaseNode)
    REGISTER_CLASS(DerivedNode)

    OutArchive ar;
    ar(node1);
    auto data = ar.GetBuffer();
    spdlog::info("{} bytes:\n{}", data.size(), data);

    InArchive iar(data);
    node1.reset();
    iar(node1);
    SPDLOG_INFO("node1: {}", *node1);

    node2 = node1->GetChild(0);
    SPDLOG_INFO("node2: {}", *node2);

    node3 = node2->GetChild(0);
    SPDLOG_INFO("node3: {}", *node3);
}

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);

    try {
        test_shared_ptr();
    } catch (const std::exception &e) {
        SPDLOG_ERROR(e.what());
    }

    return 0;
}