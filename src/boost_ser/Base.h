#pragma once
#include <dummy_base_export.h>

#include <boost/serialization/nvp.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>


#include <fmt/format.h>
#include <fmt/ranges.h>

// template <typename Archive, typename... Ts>
// void ser1(Archive &ar, Ts &&...args)
// {
//     // ar &... &args;
//     auto dummy = {(ar & BOOST_SERIALIZATION_NVP(args), 0)...};
// }

// template <typename Archive, typename T, typename... Ts>
// void ser(Archive &ar, T &&t, Ts &&...args)
// {
//     if constexpr (sizeof...(Ts) >= 1)
//         ser(ar & t, args...);
//     else
//         ar & t;
// }

class DUMMY_BASE_EXPORT Base : public std::enable_shared_from_this<Base>
{
public:
    Base() = default;

    Base(int i, float f, const std::map<int, std::string> &dict)
        : i{i}, f{f}, dict{dict}
    {
    }

    virtual ~Base();

    virtual std::string print() const;

    void save(auto &ar);
    static std::shared_ptr<Base> load(auto &ar);

    void serialize(auto &ar, const unsigned int /*version*/)
    {
        ar &BOOST_SERIALIZATION_NVP(i) & BOOST_SERIALIZATION_NVP(f)
            & BOOST_SERIALIZATION_NVP(dict);
        // ser(ar, i, f, dict);
    }

    int i;
    float f;
    std::map<int, std::string> dict;
};


template <>
struct fmt::formatter<Base> {
    constexpr auto parse(format_parse_context &ctx)
    {
        auto it = ctx.begin();
        auto end = ctx.end();
        while (it != end && *it != '}') ++it;
        return it;
    }

    auto format(const Base &b, format_context &ctx) const
    {
        return format_to(ctx.out(), "<Base at {}, i: {}, f: {:.4f}, dict: {}>",
                         fmt::ptr(&b), b.i, b.f, b.dict);
    }
};