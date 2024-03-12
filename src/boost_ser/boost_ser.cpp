#include "Derived.h"

#include <spdlog/spdlog.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/queue.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/weak_ptr.hpp>


#include <filesystem>
#include <fstream>
#include <memory>
#include <ranges>
#include <sstream>

#include <fmt/std.h>


int main()
{
    namespace fs = std::filesystem;

    auto print = [](const auto &range_of_ptr) {
        for (auto &&[i, ptr] : range_of_ptr | std::views::enumerate) {
            fmt::println("i: {}, {}", i, ptr->print());
        }
    };

    std::vector<std::shared_ptr<Base>> vec1;
    vec1.emplace_back(new Base(1, 2.2f, {{1, "a"}, {2, "b"}}));
    vec1.emplace_back(new Derived{2, 3.3f, {{3, "c"}, {4, {"d"}}}, {2, 3, 4}});
    print(vec1);

    std::ostringstream oss;
    {
        boost::archive::text_oarchive oa(oss, boost::archive::no_header);
        // oa << vec1;
        // oa << vec1[0];
        // oa << vec1[1];

        vec1[0]->save(oa);
        vec1[1]->save(oa);
    }
    auto s = oss.str();
    fmt::println("{}", s);

    std::vector<std::shared_ptr<Base>> vec2;
    std::shared_ptr<Base> b1, b2;
    {
        std::istringstream iss(s);
        boost::archive::text_iarchive ia(iss, boost::archive::no_header);
        // ia >> BOOST_SERIALIZATION_NVP(vec2);
        ia >> b1;
        ia >> b2;
    }
    spdlog::info("b1: {}", b1->print());
    spdlog::info("b2: {}", b2->print());
    // print(vec2);

    // auto pb1 = std::shared_ptr<Base>(new Base(1, 2.2f, {{1, "a"}, {2,
    // "b"}})); auto pd1 = std::shared_ptr<Base>(
    //     new Derived{2, 3.3f, {{3, "c"}, {4, {"d"}}}, {2, 3, 5}});

    // std::string s1;
    // {
    //     std::ostringstream oss1;
    //     pb1->save(oss1);
    //     spdlog::info("pb1: {}", pb1->print());
    //     pd1->save(oss1);
    //     spdlog::info("pd1: {}", pd1->print());
    //     s1 = oss1.str();
    // }
    // spdlog::info("successfully saved");

    // fmt::println("{}", s1);

    // std::shared_ptr<Base> pb2, pd2;
    // {
    //     std::istringstream iss1(s1);
    //     // std::ifstream iss1("temp.data", std::fstream::binary |
    //     // std::fstream::in);
    //     pb2 = Base::load(iss1);
    //     // pd2 = Base::load(iss1);
    // }
    // spdlog::info("pb2: {}", pb2->print());
    // // spdlog::info("pd2: {}", pd2->print());

    return 0;
}