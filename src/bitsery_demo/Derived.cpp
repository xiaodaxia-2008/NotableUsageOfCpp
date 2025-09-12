#include "Derived.h"

#include <bitsery/ext/inheritance.h>

DerivedNode::~DerivedNode() {}

void DerivedNode::serialize(auto &ar)
{
    ar(bitsery::ext::BaseClass<BaseNode>(), m_position);
}
