#include <fmt/format.h>
#include <fmt/ranges.h>

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

int main()
{
    MyData my_data;
    fmt::print("{}\n", my_data);
    MyExtendData my_ex_data;
    fmt::print("{}\n", my_ex_data);
    return 0;
}