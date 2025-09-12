#include <fmt/core.h>
#include <fmt/format.h>

#include <memory>
#include <string>

class BaseNode
{
public:
    virtual ~BaseNode() = default;

    // make it overrideable
    virtual void do_format(fmt::format_context &ctx) const
    {
        fmt::format_to(ctx.out(), "Base");
    }
};

template <std::derived_from<BaseNode> T>
struct fmt::formatter<T> {
    constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const BaseNode &b, FormatContext &ctx) const
    {
        b.do_format(ctx);
        return ctx.out();
    }
};

class DerivedNode : public BaseNode
{
    std::string name_;
    int value_;

public:
    DerivedNode(std::string name, int v) : name_(std::move(name)), value_(v) {}

    void do_format(fmt::format_context &ctx) const override
    {
        fmt::format_to(ctx.out(), "Derived{{name={}, value={}}}", name_,
                       value_);
    }
};

int main()
{
    DerivedNode d("example", 42);
    BaseNode &br = d;

    fmt::println("{}", d);
    fmt::println("{}", br);
}
