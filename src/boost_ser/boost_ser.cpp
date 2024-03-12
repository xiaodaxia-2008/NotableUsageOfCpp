#include "Derived.h"

#include <spdlog/spdlog.h>

// #include <boost/serialization/serialization.hpp>
// #include <boost/serialization/shared_ptr.hpp>
// #include <boost/serialization/weak_ptr.hpp>
// #include <boost/serialization/unique_ptr.hpp>
// #include <boost/serialization/map.hpp>
// #include <boost/serialization/vector.hpp>
// #include <boost/serialization/queue.hpp>
// #include <boost/serialization/deque.hpp>
// #include <boost/serialization/string.hpp>
// #include <boost/archive/text_oarchive.hpp>
// #include <boost/archive/text_iarchive.hpp>
// #include <boost/archive/xml_iarchive.hpp>
// #include <boost/archive/xml_oarchive.hpp>

#include <memory>
#include <sstream>
#include <ranges>


int main()
{
    auto print = [](const auto &range_of_ptr) {
        for (auto &&[i, ptr] : range_of_ptr | std::views::enumerate) {
            fmt::println("i: {}, {}", i, ptr->print());
        }
    };

    // std::vector<std::shared_ptr<Base>> vec1;
    // vec1.emplace_back(new Base(1, 2.2f, {{1, "a"}, {2, "b"}}));
    // vec1.emplace_back(new Derived{2, 3.3f, {{3, "c"}, {4, {"d"}}}, {2, 3, 4}});
    // print(vec1);

    // std::ostringstream oss;
    // {
    //     boost::archive::xml_oarchive oa(oss);
    //     oa << BOOST_SERIALIZATION_NVP(vec1);
    // }
    // auto s = oss.str();
    // fmt::println("{}", s);

    // std::vector<std::shared_ptr<Base>> vec2;
    // {
    //     std::istringstream iss(s);
    //     boost::archive::xml_iarchive ia(iss);
    //     ia >> BOOST_SERIALIZATION_NVP(vec2);
    // }
    // print(vec2);

    auto pb1 = std::shared_ptr<Base>(new Base(1, 2.2f, {{1, "a"}, {2, "b"}}));
    auto pd1 = std::shared_ptr<Base>(
        new Derived{2, 3.3f, {{3, "c"}, {4, {"d"}}}, {2, 3, 4}});

    spdlog::info("pb1: {}", pb1->print());
    spdlog::info("pd1: {}", pd1->print());

    std::ostringstream oss1;
    {
        pb1->save(oss1);
        pd1->save(oss1);
    }
    auto s1 = oss1.str();
    fmt::println("{}", s1);

    std::istringstream iss1(s1);
    auto pb2 = Base::load(iss1);
    auto pd2 = Base::load(iss1);

    spdlog::info("pb2: {}", pb2->print());
    spdlog::info("pd2: {}", pd2->print());

    return 0;
}