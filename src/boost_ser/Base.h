#pragma once
#include <dummy_base_export.h>

#include <boost/serialization/nvp.hpp>

#include <vector>
#include <string>
#include <map>
#include <memory>

#include <fmt/format.h>
#include <fmt/ranges.h>

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

    void save(std::ostream &oss);

    static std::shared_ptr<Base> load(std::istream &iss);

    void serialize(auto &ar, const unsigned int /*version*/)
    {
        ar &BOOST_SERIALIZATION_NVP(i) & BOOST_SERIALIZATION_NVP(f)
            & BOOST_SERIALIZATION_NVP(dict);
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