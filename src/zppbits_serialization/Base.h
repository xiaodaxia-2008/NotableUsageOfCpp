#pragma once
#include <zppbits_dummy_lib_base_export.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <zpp_bits.h>

class ZPPBITS_DUMMY_LIB_BASE_EXPORT Base
    : public std::enable_shared_from_this<Base>
{
public:
    Base() = default;

    Base(int i, float f, const std::map<int, std::string> &dict)
        : i{i}, f{f}, dict{dict}
    {
    }

    virtual ~Base();

    // void serialize(auto &ar, const unsigned int /*version*/);

    virtual void format(fmt::format_context &ctx) const
    {
        fmt::format_to(ctx.out(), "<Base at {}, i: {}, f: {:.4f}, dict: {}>",
                       fmt::ptr(this), i, f, dict);
    }

protected:
    int i;
    float f;
    std::map<int, std::string> dict;

private:
    friend struct zpp::bits::access;
    using serialize = zpp::bits::members<2>;
};

template <std::derived_from<Base> T>
struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    auto format(const Base &b, format_context &ctx) const
    {
        b.format(ctx);
        return ctx.out();
    }
};