#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <memory>

class Base {
public:
    virtual ~Base() = default;

    // virtual formatting hook that writes into an output iterator via fmt::format_to
    template <typename OutIt>
    void format_to(OutIt out) const {
        // default implementation
        fmt::format_to(out, "Base");
    }
    // make it overrideable
    virtual void do_format(fmt::format_context& ctx) const {
        fmt::format_to(ctx.out(), "Base");
    }
};

// inline std::string to_string(const Base& b) {
//     // helper that uses the virtual hook to produce a string
//     fmt::memory_buffer buf;
//     b.do_format(fmt::format_context(buf));
//     return fmt::to_string(buf);
// }

class Derived : public Base {
    std::string name_;
    int value_;
public:
    Derived(std::string name, int v) : name_(std::move(name)), value_(v) {}

    void do_format(fmt::format_context& ctx) const override {
        fmt::format_to(ctx.out(), "Derived{{name={}, value={}}}", name_, value_);
    }
};

// fmt::formatter specialization that delegates to the virtual hook
template <std::derived_from<Base> T>
struct fmt::formatter<T> {
    // parse is simple — accept default format spec
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Base& b, FormatContext& ctx) const {
        // call virtual formatting method which writes into ctx.out()
        b.do_format(ctx);
        return ctx.out();
    }
};

int main() {
    Derived d("example", 42);
    Base& br = d;

    // polymorphic formatting via fmt::format
    auto s1 = fmt::format("{}", d);   // "Derived{name=example, value=42}"
    auto s2 = fmt::format("{}", br);  // likewise, uses Derived::do_format

    // or directly:
    fmt::print("{}\n", br);
}
