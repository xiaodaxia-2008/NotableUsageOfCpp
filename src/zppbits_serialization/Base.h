#pragma once
#include <zppbits_dummy_lib_base_export.h>

#include "Archive.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>

#define SIMPLE_REFLECTION(T)                                                   \
public:                                                                        \
    virtual std::string_view GetClassName() const { return #T; }

class ZPPBITS_DUMMY_LIB_BASE_EXPORT BaseNode
    : public std::enable_shared_from_this<BaseNode>
{
    SIMPLE_REFLECTION(BaseNode)

public:
    BaseNode() = default;

    BaseNode(std::string name) : m_name(std::move(name)) {}

    virtual ~BaseNode();

    std::shared_ptr<BaseNode> GetParent() const { return m_parent.lock(); }

    void SetName(std::string name) { m_name = std::move(name); }

    std::string_view GetName() const { return m_name; }

    void AddChild(std::shared_ptr<BaseNode> child)
    {
        if (std::ranges::find(m_children, child) == m_children.end()) {
            child->m_parent = shared_from_this();
            m_first_child = child;
            m_first_child_weak = child;
            m_children.push_back(std::move(child));
        }
    }

    void RemoveChild(const std::shared_ptr<BaseNode> &child)
    {
        auto it = std::ranges::find(m_children, child);
        if (it != m_children.end()) {
            m_children.erase(it);
            child->m_parent.reset();
        }
    }

    std::shared_ptr<BaseNode> GetChild(std::size_t index)
    {
        return m_children.at(index);
    }

    std::size_t GetChildrenCount() const { return m_children.size(); }

    virtual void serialize(OutArchive &ar) const;

    virtual void serialize(InArchive &ar);

    virtual void format(fmt::format_context &ctx) const
    {
        auto parent = GetParent();
        fmt::format_to(
            ctx.out(),
            "<{} {} at {} with {} children, parent {}, {}, first: {}, "
            "first weak: {}>",
            GetClassName(), GetName(), fmt::ptr(this), GetChildrenCount(),
            parent ? parent->GetName() : "null",
            m_children.empty() ? fmt::ptr((void*)0)
                               : fmt::ptr(m_children.front().get()),
            fmt::ptr(m_first_child.get()),
            fmt::ptr(m_first_child_weak.lock().get()));
    }

protected:
    std::string m_name{"Node"};

    std::vector<std::shared_ptr<BaseNode>> m_children;

    std::weak_ptr<BaseNode> m_parent;
    std::shared_ptr<BaseNode> m_first_child;
    std::weak_ptr<BaseNode> m_first_child_weak;
};

template <std::derived_from<BaseNode> T>
struct fmt::formatter<T> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    auto format(const BaseNode &b, format_context &ctx) const
    {
        b.format(ctx);
        return ctx.out();
    }
};