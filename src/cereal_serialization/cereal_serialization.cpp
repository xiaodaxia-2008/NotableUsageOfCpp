#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <strstream>
#include <unordered_map>
#include <vector>

struct MyData
{
    std::string name{"Data"};
    std::vector<double> vec_dbl{1.0, 2.0, 3.0};
    std::unordered_map<std::string, double> umap{{"A", 1.0}, {"B", 2.0}};
};

struct MyExtendData : public MyData
{
    std::string extend_value{"Derived"};
};

template <> struct fmt::formatter<MyData> : fmt::formatter<std::string>
{
    template <typename FormatContext> auto format(const MyData &m, FormatContext &ctx)
    {
        return fmt::format_to(ctx.out(), "<MyData at {}, name: {}, vec_dbl: {}, umap: {}>", fmt::ptr(&m), m.name,
                              m.vec_dbl, m.umap);
    }
};

template <> struct fmt::formatter<MyExtendData> : fmt::formatter<std::string>
{
    template <typename FormatContext> auto format(const MyExtendData &m, FormatContext &ctx)
    {
        return fmt::format_to(ctx.out(), "<MyExtendData at {}, name: {}, vec_dbl: {}, umap: {}, extend_value: {}>",
                              fmt::ptr(&m), m.name, m.vec_dbl, m.umap, m.extend_value);
    }
};

template <class Archive> void serialize(Archive &archive, MyData &m)
{
    archive(CEREAL_NVP(m.name), CEREAL_NVP(m.vec_dbl), CEREAL_NVP(m.umap));
}

template <class Archive> void serialize(Archive &archive, MyExtendData &m)
{
    archive(cereal::base_class<MyData>(&m));
    archive(CEREAL_NVP(m.extend_value));
}

int main()
{
    MyData my_data;
    fmt::print("{}\n", my_data);
    MyExtendData my_ex_data;
    fmt::print("{}\n", my_ex_data);

    auto fname = std::filesystem::current_path() / "ser.data";

    {
        std::ofstream ofs(fname, std::ios::binary);
        cereal::BinaryOutputArchive ar(ofs);
        ar(my_data);
        ar(my_ex_data);
    }
    {
        std::ifstream ifs(fname, std::ios::binary);
        cereal::BinaryInputArchive ar(ifs);
        MyData my_data1;
        MyExtendData my_ex_data1;
        ar(my_data1);
        ar(my_ex_data1);
        fmt::print("{}\n", my_data1);
        fmt::print("{}\n", my_ex_data1);
    }
    return 0;
}