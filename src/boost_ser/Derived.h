#pragma once
#include "Base.h"

#include <dummy_derived_export.h>

#include <deque>

#include <boost/serialization/base_object.hpp>

class DUMMY_DERIVED_EXPORT Derived : public Base
{
public:
    Derived() = default;

    Derived(int i, float f, const std::map<int, std::string> &dict,
            std::deque<int> queue)
        : Base(i, f, dict), queue(queue)
    {
    }

    ~Derived();

    virtual std::string print() const;

    void serialize(auto &ar, const unsigned int)
    {
        ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base);
        ar &BOOST_SERIALIZATION_NVP(queue);
    }

    std::deque<int> queue{1, 2, 3};
};

template <>
struct fmt::formatter<Derived> : formatter<Base> {
    auto format(const Derived &d, format_context &ctx) const
    {
        return format_to(
            ctx.out(), "<Derived at {}, i: {}, f: {:.4f}, dict: {}, queue: {}>",
            fmt::ptr(&d), d.i, d.f, d.dict, d.queue);
    }
};