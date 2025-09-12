#include "Derived.h"

#include <zpp_bits.h>

#include <rttr/registration>

DerivedNode::~DerivedNode() {}

void DerivedNode::save(OutputArchive &ar) const
{
    BaseNode::save(ar);
    ar(queue);
}

void DerivedNode::load(InputArchive &ar)
{
    BaseNode::load(ar);
    ar(queue);
}

RTTR_REGISTRATION
{
    rttr::registration::class_<DerivedNode>("Derived").constructor<>()(
        rttr::policy::ctor::as_std_shared_ptr);
}