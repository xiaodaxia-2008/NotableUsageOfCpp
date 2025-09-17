#include "Archive.h"
#include "Derived.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>
#include <stacktrace>

#include <fmt/ranges.h>

#define NVP(x) zen::make_nvp(#x, x)

struct Person {
    std::string name{"John Doe"};
    int age{18};
    double weight{80.5};
    std::vector<std::string> children{"Alice", "Bob"};

    SERIALIZE_MEMBER(name, age, weight, children)
};

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

    std::stringstream ss;
    {
        OutArchive ar(ss);
        ar(node1);
    }

    auto data = ss.str();
    SPDLOG_INFO("bytes size: {}\n{}", data.size(), data);

    // std::vector<char> bytes{data.begin(), data.end()};

    // SPDLOG_INFO("bytes size: {}\n{}", bytes.size(), bytes);

    // InArchive iar(ss);
    // node1.reset();
    // iar(node1);
    // SPDLOG_INFO("node1: {}", *node1);

    // node2 = node1->GetChild(0);
    // SPDLOG_INFO("node2: {}", *node2);

    // node3 = node2->GetChild(0);
    // SPDLOG_INFO("node3: {}", *node3);
}

namespace fs = std::filesystem;
int main()
{
    std::set_terminate([] {
        auto curexp = std::current_exception();
        if (curexp) {
            try {
                spdlog::error("{}", fmt::streamed(std::stacktrace::current()));
                std::rethrow_exception(curexp);
            } catch (std::exception &e) {
                spdlog::error("{}", e.what());
            }
        }
    });

    spdlog::set_level(spdlog::level::debug);
    test_shared_ptr();

    // Person john{.name = "John", .age = 20, .weight = 80.8};
    // std::shared_ptr<Person> mike = std::shared_ptr<Person>(
    //     new Person{.name = "Mike", .age = 20, .weight = 80.8});

    // std::stringstream ss;
    // {
    //     OutArchive oar(ss);
    //     oar(NVP(john));
    //     oar(NVP(mike));
    // }

    // SPDLOG_INFO("Serialized: {}", ss.str());

    // Person p2;

    return 0;
}