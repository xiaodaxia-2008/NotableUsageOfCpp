#include "Base.h"

#include <bitsery/brief_syntax.h>

BaseNode::~BaseNode() {}

void BaseNode::serialize(auto &ar) { ar(m_name, m_children, m_parent); }
