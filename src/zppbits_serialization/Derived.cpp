#include "Derived.h"

#include <zpp_bits.h>

#include <rttr/registration>

Derived::~Derived() {}

void Derived::save(OutputArchive &ar) const
{
    Base::save(ar);
    ar(queue);
}

void Derived::load(InputArchive &ar)
{
    Base::load(ar);
    ar(queue);
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Derived>("Derived").constructor<>()(
        rttr::policy::ctor::as_std_shared_ptr);
}