#pragma once
#include <spdlog/spdlog.h>

#include <zen_serialization_export.h>

#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <typeindex>
#include <variant>

namespace zen
{

using TByte = char;
using TBuffer = std::vector<char>;

class OutArchive;
class InArchive;

class Access
{
public:
    template <typename T>
    static T *Create()
    {
        return new T;
    }
};

struct ZEN_SERIALIZATION_EXPORT ArchiveBase {
    template <typename T>
    static void RegisterClassName(const std::string &name)
    {
        RegisterClassName(std::type_index(typeid(T)), name);
    }

    static void RegisterClassName(const std::type_index &index,
                                  const std::string &name);

    template <typename T>
    static const std::string &GetClassName()
    {
        return GetClassName(std::type_index(typeid(T)));
    }

    static const std::string &GetClassName(const std::type_index &index);

    template <typename T>
    static void RegisterClass(const std::string &class_name)
    {
        RegisterClassName<T>(class_name);
        RegisterConstructor<T>();
        RegisterSerializer<T>();
        RegisterDeserializer<T>();
    }

    template <typename T>
    static void RegisterConstructor()
    {
        RegisterConstructor(GetClassName<T>(),
                            [] -> void * { return Access::Create<T>(); });
    }

    static void RegisterConstructor(const std::string &name,
                                    std::function<void *()> constructor);

    static const std::function<void *()> &
    GetConstructor(const std::string &name);

    template <typename T>
    static void RegisterSerializer()
    {
        const auto &type_name = GetClassName<T>();
        if constexpr (requires(T t) {
                          t.serialize(std::declval<OutArchive &>());
                      }) {
            RegisterSerializer(type_name, [](void *ptr, OutArchive &ar) {
                static_cast<T *>(ptr)->serialize(ar);
            });
        } else {
            static_assert(false, "type must have a serialize method");
        }
    }

    template <typename T>
    static void RegisterDeserializer()
    {
        const auto &type_name = GetClassName<T>();
        if constexpr (requires(T t) {
                          t.serialize(std::declval<InArchive &>());
                      }) {
            RegisterDeserializer(type_name, [](void *ptr, InArchive &ar) {
                static_cast<T *>(ptr)->serialize(ar);
            });
        } else {
            static_assert(false, "type must have a deserialize method");
        }
    }

    static void
    RegisterSerializer(const std::string &name,
                       std::function<void(void *, OutArchive &ar)> serializer);

    static void RegisterDeserializer(
        const std::string &name,
        std::function<void(void *, InArchive &ar)> deserializer);

    static const std::function<void(void *, OutArchive &ar)> &
    GetSerializer(const std::string &name);

    static const std::function<void(void *, InArchive &ar)> &
    GetDeserializer(const std::string &name);

    template <typename T>
    static T *Create(const std::string &name)
    {
        try {
            return static_cast<T *>(GetConstructor(name)());
        } catch (...) {
            if constexpr (std::is_default_constructible_v<T> &&
                          !std::is_polymorphic_v<T>) {
                return new T();
            }
        }

        throw std::runtime_error(fmt::format("No constructor for {} ", name));
    }
};

class ZEN_SERIALIZATION_EXPORT OutArchive : public ArchiveBase
{
    TBuffer m_buffer;
    std::set<std::uintptr_t> m_pointers;

public:
    constexpr bool IsOutput() const { return true; }

    OutArchive() {}

    const TBuffer &GetBuffer() const { return m_buffer; }

    void operator()(auto &&item1, auto &&...items)
    {
        serialize(item1);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

private:
    template <typename T>
    void serialize(const T &item)
    {
        if constexpr (std::is_arithmetic_v<T>) {
            auto ptr = reinterpret_cast<const TByte *>(&item);
            m_buffer.insert(m_buffer.end(), ptr, ptr + sizeof(T));
        } else if constexpr (std::is_enum_v<T>) {
            serialize(static_cast<std::underlying_type_t<T>>(item));
        } else if constexpr (requires(T item) { item.serialize(*this); }) {
            const_cast<T &>(item).serialize(*this);
        } else {
            throw std::runtime_error(
                fmt::format("Unsupported type {}", typeid(T).name()));
        }
    }

    template <std::ranges::range Rng>
    void serialize(const Rng &items)
    {
        using T = std::ranges::range_value_t<Rng>;
        std::size_t n = items.size();
        serialize(n);
        if constexpr (std::ranges::contiguous_range<Rng> &&
                      std::is_arithmetic_v<T>) {
            auto ptr = reinterpret_cast<const TByte *>(items.data());
            m_buffer.insert(m_buffer.end(), ptr, ptr + n * sizeof(T));
        } else {
            for (const auto &i : items) {
                serialize(i);
            }
        }
    }

    template <typename T>
    void serialize(const std::unique_ptr<T> &item)
    {
        serialize(item.get());
    }

    template <typename T>
    void serialize(const std::weak_ptr<T> &item)
    {
        serialize(item.lock().get());
    }

    template <typename T>
    void serialize(const std::shared_ptr<T> &item)
    {
        serialize(item.get());
    }

    template <typename T>
        requires std::is_pointer_v<T>
    void serialize(const T &item)
    {
        auto id = reinterpret_cast<std::uintptr_t>(item);
        serialize(id);
        if (id == 0) {
            return;
        }

        if (m_pointers.contains(id)) {
            return;
        }
        constexpr bool is_polymorphic =
            std::is_polymorphic_v<std::remove_pointer_t<T>>;

        m_pointers.insert(id);
        if constexpr (!is_polymorphic) {
            serialize(*item);
            return;
        } else {
            const auto &type_name = GetClassName(typeid(*item));
            serialize(type_name);
            const auto &serializer = GetSerializer(type_name);
            serializer(item, *this);
        }
    }
};

class ZEN_SERIALIZATION_EXPORT InArchive : public ArchiveBase
{
    const TBuffer &m_buffer;
    std::size_t m_index = 0;

    std::map<std::uintptr_t, std::shared_ptr<void>> m_pointers;

public:
    InArchive(const TBuffer &buffer) : m_buffer(buffer) {}

    void operator()(auto &&item1, auto &&...items)
    {
        serialize(item1);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

private:
    template <typename T>
    void serialize(T &item)
    {
        if constexpr (std::is_arithmetic_v<T>) {
            auto ptr = reinterpret_cast<TByte *>(&item);
            std::copy_n(&m_buffer[m_index], sizeof(T), ptr);
            m_index += sizeof(T);
        } else if constexpr (std::is_enum_v<T>) {
            std::underlying_type_t<T> value;
            serialize(value);
            item = static_cast<T>(value);
        } else if constexpr (requires(T item) { item.serialize(*this); }) {
            item.serialize(*this);
        } else {
            throw std::runtime_error(
                fmt::format("Unsupported type {} ", typeid(T).name()));
        }
    }

    template <std::ranges::range Rng>
    void serialize(Rng &items)
    {
        using T = std::ranges::range_value_t<decltype(items)>;
        std::size_t n;
        serialize(n);
        if constexpr (requires { items.reserve(n); }) {
            items.reserve(n);
        }

        if constexpr (std::ranges::contiguous_range<Rng> &&
                      std::is_arithmetic_v<T>) {
            if constexpr (requires { items.resize(n); }) {
                items.resize(n);
            } else {
                assert(items.size() == n);
            }
            auto ptr = reinterpret_cast<TByte *>(items.data());
            std::copy_n(&m_buffer[m_index], n * sizeof(T), ptr);
            m_index += n * sizeof(T);
        } else {
            for (std::size_t i = 0; i < n; ++i) {
                T item;
                serialize(item);
                if constexpr (requires { items.push_back(item); }) {
                    items.push_back(std::move(item));
                } else {
                    items[i] = std::move(item);
                }
            }
        }
    }

    template <typename T>
    void serialize(std::unique_ptr<T> &item)
    {
        std::uintptr_t id;
        serialize(id);
        if (id == 0) {
            item.reset();
            return;
        }

        if constexpr (std::is_polymorphic_v<T>) {
            std::string type_name;
            serialize(type_name);
            item = std::unique_ptr<T>(Create<T>(type_name));

            const auto &deserializer = GetDeserializer(type_name);
            deserializer(item.get(), *this);

        } else {
            item = std::unique_ptr<T>(Access::Create<T>());
            serialize(*item);
        }
    }

    template <typename T>
    void serialize(std::weak_ptr<T> &item)
    {
        std::shared_ptr<T> shared;
        serialize(shared);
        item = shared;
    }

    template <typename T>
    void serialize(std::shared_ptr<T> &item)
    {
        std::uintptr_t id;
        serialize(id);
        if (id == 0) {
            item.reset();
            return;
        }
        if (m_pointers.contains(id)) {
            item = std::static_pointer_cast<T>(m_pointers[id]);
            return;
        }

        if constexpr (std::is_polymorphic_v<T>) {
            std::string type_name;
            serialize(type_name);
            item = std::shared_ptr<T>(Create<T>(type_name));
            m_pointers[id] = item;

            const auto &deserializer = GetDeserializer(type_name);
            deserializer(item.get(), *this);
        } else {
            item = std::shared_ptr<T>(Access::Create<T>());
            m_pointers[id] = item;
            serialize(*item);
        }
    }
};

} // namespace zen

#define REGISTER_CLASS(T) zen::ArchiveBase::RegisterClass<T>(#T);