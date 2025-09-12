#include "Base.h"
#include "Archive.h"

BaseNode::~BaseNode() {}

void BaseNode::serialize(OutArchive &ar) const
{
    // ar(m_name, m_children, m_parent);
    ar(m_name);
    ar(m_parent);
}

void BaseNode::serialize(InArchive &ar)
{
    ar(m_name);
    ar(m_parent);
}
