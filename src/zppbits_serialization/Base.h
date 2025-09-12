#pragma once
#include <zppbits_dummy_lib_base_export.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "archive.h"

class ZPPBITS_DUMMY_LIB_BASE_EXPORT BaseNode
    : public std::enable_shared_from_this<BaseNode>
{
    RTTR_ENABLE()

public:
    BaseNode() = default;

    BaseNode(int i, float f, const std::map<int, std::string> &dict)
        : i{i}, f{f}, dict{dict}
    {
    }

    virtual ~BaseNode();

    // void serialize(auto &ar, const unsigned int /*version*/);

    virtual void save(OutputArchive &ar) const;
    virtual void load(InputArchive &ar);

    virtual void format(fmt::format_context &ctx) const
    {
        fmt::format_to(ctx.out(), "<Base at {}, i: {}, f: {:.4f}, dict: {}>",
                       fmt::ptr(this), i, f, dict);
    }

protected:
    int i;
    float f;
    std::map<int, std::string> dict;
};

template <std::derived_from<BaseNode> T>
struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    auto format(const BaseNode &b, format_context &ctx) const
    {
        b.format(ctx);
        return ctx.out();
    }
};