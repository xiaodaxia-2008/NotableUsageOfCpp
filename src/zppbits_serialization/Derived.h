#pragma once
#include "Base.h"

#include <zppbits_dummy_lib_derived_export.h>

#include <array>
#include <span>

class ZPPBITS_DUMMY_LIB_DERIVED_EXPORT DerivedNode : public BaseNode
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

    void serialize(OutArchive &ar) const override;
    void serialize(InArchive &ar) override;

    void format(fmt::format_context &ctx) const override
    {
        auto parent = GetParent();
        fmt::format_to(ctx.out(),
                       "<{} {} at {} with {} children, parent {}, position "
                       "[{:.4g}, {:.4g}, {:.4g}]>",
                       GetClassName(), GetName(), fmt::ptr(this),
                       GetChildrenCount(), parent ? parent->GetName() : "null",
                       m_position[0], m_position[1], m_position[2]);
    }

protected:
    std::array<float, 3> m_position;
};
