#include "reflection.h"

namespace refl
{

Type g_bad_type;

struct Registry::Pimpl {
    std::unordered_map<std::type_index, std::string> rtti_to_type_names;
    std::unordered_map<std::string, std::unique_ptr<Type>> type_names_to_types;
};

Registry::Registry() : pimpl(std::make_unique<Pimpl>()) {}

Registry::~Registry() = default;

Registry *Registry::Get()
{
    static Registry instance;
    return &instance;
}

Type &Registry::GetClass(const std::string &name)
{
    if (auto it = pimpl->type_names_to_types.find(name);
        it != pimpl->type_names_to_types.end()) {
        return *it->second;
    }

    return g_bad_type;
}

Type &Registry::GetClass(const std::type_index &ti)
{
    if (auto it = pimpl->rtti_to_type_names.find(ti);
        it != pimpl->rtti_to_type_names.end()) {
        return GetClass(it->second);
    }
    return g_bad_type;
}

Type &Registry::GetClassRtti(const std::type_index &ti)
{
    if (auto it = pimpl->rtti_to_type_names.find(ti);
        it != pimpl->rtti_to_type_names.end()) {
        return GetClass(it->second);
    }
    return g_bad_type;
}

Type &Registry::AddClassRtti(const std::type_index &ti,
                             const std::string &type_name)
{
    pimpl->rtti_to_type_names[ti] = type_name;
    pimpl->type_names_to_types[type_name] = std::make_unique<Type>(type_name);
    return *pimpl->type_names_to_types[type_name];
}
} // namespace refl