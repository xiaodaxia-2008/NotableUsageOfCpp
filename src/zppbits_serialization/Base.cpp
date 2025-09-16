#include "Base.h"
#include "Archive.h"

BaseNode::~BaseNode() {}

template <class Archive>
void BaseNode::serialize(Archive &ar)
{
    ar(m_name, m_parent, m_first_child_weak, m_first_child, m_children, m_id);
}

template ZPPBITS_DUMMY_LIB_BASE_EXPORT void BaseNode::serialize(OutArchive &ar);
template ZPPBITS_DUMMY_LIB_BASE_EXPORT void BaseNode::serialize(InArchive &ar);
