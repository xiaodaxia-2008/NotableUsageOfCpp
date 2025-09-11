#pragma once

#include <fmt/core.h>

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <typeindex>

namespace refl
{
using any = std::any;
using std::any_cast;

struct Property {
    std::string_view name;
    std::function<any(any &)> getter;
    std::function<void(any &, any)> setter;
};

struct Method {
    std::string_view name;
    std::function<any(any &, std::span<any>)> functor;
};

struct Type {
    std::string_view name;
    any (*default_contructor)();

    std::map<std::string_view, Property> properties;
    std::map<std::string_view, Method> methods;

    bool IsValid() const { return !name.empty(); }

    any Create()
    {
        if (default_contructor) {
            return default_contructor();
        }
        throw std::runtime_error("No default constructor");
    }

    template <typename T,
              template <class _T> typename Wrapper = std::shared_ptr>
    Wrapper<T> Create()
    {
        return Wrapper<T>(any_cast<T *>(Create()));
    }
};

template <typename C>
struct TypeBuilder {
    Type *type;

    TypeBuilder &SupportDefaultConstructor()
    {
        if constexpr (std::is_default_constructible_v<C>) {
            type->default_contructor = []() -> any { return new C; };
        }
        return *this;
    }

    template <typename T>
    TypeBuilder &AddProperty(std::string_view name, T C::*member)
    {
        Property property;
        property.name = name;
        property.getter = [member](any &object) -> std::any {
            return any_cast<C &>(object).*member;
        };

        property.setter = [member](any &object, std::any value) {
            any_cast<C &>(object).*member = std::any_cast<T>(value);
        };

        type->properties.emplace(name, std::move(property));

        return *this;
    }

    template <typename R, typename... Args>
    TypeBuilder &AddMethod(std::string_view name, R (C::*member)(Args...))
    {
        Method method;
        method.name = name;
        method.functor = [member](any &object,
                                  std::span<const std::any> args) -> std::any {
            if (sizeof...(Args) != args.size()) {
                throw std::runtime_error("Mismatched number of arguments");
            }
            return
                [&]<std::size_t... I>(std::index_sequence<I...>) -> std::any {
                    if constexpr (std::is_void_v<R>) {
                        (any_cast<C &>(object).*
                         member)(std::any_cast<Args>(args[I])...);
                        return std::any{};
                    } else {
                        return (any_cast<C &>(object).*
                                member)(std::any_cast<Args>(args[I])...);
                    }
                }(std::index_sequence_for<Args...>{});
        };

        type->methods.emplace(name, std::move(method));

        return *this;
    }

    template <typename R, typename... Args>
    TypeBuilder &AddMethod(std::string_view name, R (C::*member)(Args...) const)
    {
        Method method;
        method.name = name;
        method.functor = [member](any &object,
                                  std::span<const std::any> args) -> std::any {
            if (sizeof...(Args) != args.size()) {
                throw std::runtime_error("Mismatched number of arguments");
            }
            return
                [&]<std::size_t... I>(std::index_sequence<I...>) -> std::any {
                    if constexpr (std::is_void_v<R>) {
                        (any_cast<const C &>(object).*
                         member)(std::any_cast<Args>(args[I])...);
                        return std::any{};
                    } else {
                        return (any_cast<const C &>(object).*
                                member)(std::any_cast<Args>(args[I])...);
                    }
                }(std::index_sequence_for<Args...>{});
        };

        type->methods.emplace(name, std::move(method));

        return *this;
    }
};

class Registry
{
public:
    Registry();
    ~Registry();

    static Registry *Get();

    template <typename T>
    TypeBuilder<T> AddClass(std::string name)
    {
        auto &type = AddClassRtti(std::type_index(typeid(T)), name);
        return TypeBuilder<T>{&type};
    }

    Type &GetClass(const std::string &name);

    Type &GetClass(const std::type_index &ti);

    template <typename T>
    Type &GetClass()
    {
        return GetClassRtti(std::type_index(typeid(T)));
    }

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    Type &AddClassRtti(const std::type_index &ti, const std::string &type_name);
    Type &GetClassRtti(const std::type_index &ti);
};

template <typename T>
void SetProperty(std::any &instance, std::string_view property_name,
                 const T &value)
{
    auto &type = Registry::Get()->GetClass(instance.type());
    if (!type.IsValid()) {
        throw std::runtime_error("Not registered type");
    }

    auto it = type.properties.find(property_name);
    if (it == type.properties.end()) {
        throw std::runtime_error("Not registered property");
    }

    it->second.setter(instance, value);
}

std::any GetProperty(std::any &instance, std::string_view property_name,
                     std::any &value);
} // namespace  refl

template <>
struct fmt::formatter<refl::Type> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    auto format(const refl::Type &type, format_context &ctx) const
    {
        return format_to(ctx.out(), "{}", type.name);
    }
};