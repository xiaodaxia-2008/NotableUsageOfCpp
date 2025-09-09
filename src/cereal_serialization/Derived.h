#pragma once
#include "Base.h"

#include <dummy_lib_derived_export.h>

#include <deque>

class DUMMY_LIB_DERIVED_EXPORT Derived : public Base
{
public:
    Derived() = default;

    Derived(int i, float f, const std::map<int, std::string> &dict,
            std::deque<int> queue)
        : Base(i, f, dict), queue(queue)
    {
    }

    ~Derived();

    void serialize(auto &ar, const unsigned int);

    void format(fmt::format_context &ctx) const override
    {
        fmt::format_to(ctx.out(),
                       "<Derived at {}, i: {}, f: {:.4f}, dict: {}, queue: {}>",
                       fmt::ptr(this), i, f, dict, queue);
    }

protected:
    std::deque<int> queue{1, 2, 3};
};
