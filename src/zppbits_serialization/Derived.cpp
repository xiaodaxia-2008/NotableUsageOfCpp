#include "Derived.h"

#include "Archive.h"

DerivedNode::~DerivedNode() {}

void DerivedNode::serialize(OutArchive &ar) const
{
    BaseNode::serialize(ar);
    ar(m_position);
}

void DerivedNode::serialize(InArchive &ar)
{
    BaseNode::serialize(ar);
    ar(m_position);
}
