#pragma once
#include "Base.h"

#include <zppbits_dummy_lib_derived_export.h>

#include <deque>

class ZPPBITS_DUMMY_LIB_DERIVED_EXPORT Derived : public Base
{
    RTTR_ENABLE(Base)

public:
    Derived() = default;

    Derived(int i, float f, const std::map<int, std::string> &dict,
            std::deque<int> queue)
        : Base(i, f, dict), queue(queue)
    {
    }

    ~Derived();

    // void serialize(auto &ar, const unsigned int);

    void save(OutputArchive &ar) const override;
    void load(InputArchive &ar) override;

    void format(fmt::format_context &ctx) const override
    {
        fmt::format_to(ctx.out(),
                       "<Derived at {}, i: {}, f: {:.4f}, dict: {}, queue: {}>",
                       fmt::ptr(this), i, f, dict, queue);
    }

protected:
    std::deque<int> queue{1, 2, 3};

private:
    friend struct zpp::bits::access;
    using serialize = zpp::bits::members<1>;
};
