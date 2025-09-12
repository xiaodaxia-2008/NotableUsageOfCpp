#pragma once
#include "Base.h"

#include <bitsery_lib_derived_export.h>

#include <array>
#include <span>

class BITSERY_LIB_DERIVED_EXPORT DerivedNode : public BaseNode
{
    SIMPLE_REFLECTION(DerivedNode)
public:
    DerivedNode() = default;

    DerivedNode(std::string name, std::array<float, 3> position)
        : BaseNode(std::move(name)), m_position(std::move(position))
    {
    }

    const std::array<float, 3> &GetPosition() const { return m_position; }

    void SetPosition(std::array<float, 3> position)
    {
        m_position = std::move(position);
    }

    ~DerivedNode();

    void serialize(auto &ar);

protected:
    std::array<float, 3> m_position;
};
