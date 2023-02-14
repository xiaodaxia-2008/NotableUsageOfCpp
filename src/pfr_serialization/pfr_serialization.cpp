#include <boost/pfr.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace pfr = boost::pfr;

struct Shop
{
    std::string name{"Zen's Shop"};
    int open_time{9};
    int close_time{21};
};

struct some_person
{
    std::string name{"Name"};
    unsigned birth_year{89};
    std::vector<double> vec_dbl{1.0, 2.0, 3.0};
    std::unordered_map<std::string, double> umap{{"A", 1.0}, {"B", 2.0}};
};

template <typename T>
    requires std::is_aggregate_v<T> && (!fmt::detail::is_streamable<T, char>::value)
struct fmt::formatter<T> : fmt::formatter<std::string>
{
    template <typename FormatContext> auto format(const T &m, FormatContext &ctx)
    {
        auto out = ctx.out();
        fmt::format_to(out, "{} {{ ", typeid(m).name());
        pfr::for_each_field(m, [&](auto &&v) { fmt::format_to(out, "{}, ", v); });
        fmt::format_to(out, " }}");
        return out;
    }
};

namespace cereal
{
template <class Archive, class T>
    requires std::is_aggregate_v<T>
void serialize(Archive &ar, T &o)
{
    pfr::for_each_field(o, [&](auto &&v) { ar(v); });
}
} // namespace cereal

int main(int argc, char **argv)
{
    some_person person{"Edgar Allan Poe", 1809};
    Shop shop;
    fmt::print("size of some_persion: {}\n", pfr::tuple_size_v<some_person>);
    fmt::print("{}\n", person);
    fmt::print("{}\n", shop);

    auto fname = std::filesystem::current_path() / "ser.data";

    {
        std::ofstream ofs(fname);
        cereal::JSONOutputArchive ar(ofs);
        ar(person);
        ar(shop);
    }
    {
        std::ifstream ifs(fname);
        cereal::JSONInputArchive ar(ifs);
        some_person person1;
        Shop shop1;
        ar(person1);
        ar(shop1);
        fmt::print("{}\n", person1);
        fmt::print("{}\n", shop1);
    }
    return 0;
}