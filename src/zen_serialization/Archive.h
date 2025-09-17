#pragma once
#include <spdlog/spdlog.h>

#include <zenser_export.h>

#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <typeindex>
#include <variant>

#include <nlohmann/json.hpp>

#define PARENS ()
#define EXPAND(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...)                                                   \
    __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                                        \
    macro(a1) __VA_OPT__(, FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define SERIALIZE_MEMBER(...)                                                  \
    template <typename Archive>                                                \
    void serialize(Archive &ar)                                                \
    {                                                                          \
        ar(FOR_EACH(NVP, __VA_ARGS__));                                        \
    }

#define REGISTER_CLASS(T) zen::ArchiveBase::RegisterClass<T>(#T);

namespace zen
{
struct RangeSize {
    explicit RangeSize(std::size_t size) : size(size) {}
    std::size_t size;
};

class Scope
{
    std::function<void()> f;

public:
    Scope(std::function<void()> f) : f(std::move(f)) {}

    ~Scope()
    {
        if (f) {
            f();
        }
    }
};

class JsonSerializer
{
    std::stack<nlohmann::json> m_objects;
    std::size_t m_idx = 0;

    std::stack<std::string> m_next_names;

public:
    static constexpr bool is_binary = false;

    JsonSerializer() { m_objects.emplace(nlohmann::json::object()); }

    const nlohmann::json &Json() const
    {
        assert(m_objects.size() == 1);
        return m_objects.top();
    }

    void SetNextName(std::string_view name) { m_next_names.emplace(name); }

    std::string NextName()
    {
        if (m_next_names.empty()) {
            return std::format("value{}", m_idx++);
        } else {
            std::string name = std::move(m_next_names.top());
            m_next_names.pop();
            return std::move(name);
        }
    }

    void NewObject() { m_objects.emplace(nlohmann::json::object()); }

    void FinishObject()
    {
        auto current = std::move(m_objects.top());
        m_objects.pop();
        m_objects.top()[NextName()] = std::move(current);
    }

    void NewArray() { m_objects.emplace(nlohmann::json::array()); }

    void operator()(const RangeSize &size) {}

    template <typename T>
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
    void operator()(const T &t)
    {
        auto &current = m_objects.top();
        if (current.is_object()) {
            current[NextName()] = t;
        } else if (current.is_array()) {
            current.push_back(t);
        } else {
            throw std::runtime_error("Invalid json type");
        }
    }
};

class JsonDeserializer
{
    nlohmann::json m_json;

public:
    JsonDeserializer(std::string_view json)
    {
        m_json = nlohmann::json::parse(json);
    }

    static constexpr bool is_binary = false;

    template <typename T>
        requires std::is_arithmetic_v<T>
    void operator()(std::string_view name, T &t)
    {
        t = m_json[name].get<T>();
    }
};
} // namespace zen
namespace zen
{

class OutArchive;
class InArchive;

template <typename T>
struct NamedValuePair {
    std::string name;
    T value;
};

template <typename T>
NamedValuePair<T> make_nvp(std::string name, T &&value)
{
    return NamedValuePair<T>{std::move(name), std::forward<T>(value)};
}

class Access
{
public:
    template <typename T>
    static T *Create()
    {
        return new T;
    }
};

struct ZENSER_EXPORT ArchiveBase {
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

class ZENSER_EXPORT OutArchive : public ArchiveBase
{
public:
    using TSerializer = JsonSerializer;

private:
    std::set<std::uintptr_t> m_pointers;

    std::ostream &m_stream;

    TSerializer m_serializer;

public:
    constexpr bool IsOutput() const { return true; }

    OutArchive(std::ostream &stream) : m_stream(stream) {}

    ~OutArchive() { Flush(); }

    void Flush() { m_stream << m_serializer.Json().dump(2); }

    void operator()(auto &&item1, auto &&...items)
    {
        try_serialize(item1);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

private:
    template <bool IsArray = false>
    struct NewJsonScope {
        TSerializer &serializer;
        Scope scope;
        NewJsonScope(TSerializer &ser)
            : serializer(ser), scope([&] { serializer.FinishObject(); })
        {
            if constexpr (IsArray) {
                serializer.NewArray();
            } else {
                serializer.NewObject();
            }
        }
    };

    template <typename T>
    void try_serialize(const NamedValuePair<T> &item)
    {
        m_serializer.SetNextName(item.name);

        // bool new_object = false;

        // if constexpr (std::ranges::range<T> ) {
        //     m_serializer.NewArray();
        // } else if constexpr (std::is_class_v<T>) {
        //     m_serializer.NewObject();
        // }
        try_serialize(item.value);
        // if constexpr (std::is_class_v<T>) {
        //     m_serializer.FinishObject();
        // }
    }

    template <typename T>
    void try_serialize(const T &item)
    {
        if constexpr (requires(const T item) { item.save(*this); }) {
            NewJsonScope scope(m_serializer);
            item.save(*this);
        } else if constexpr (requires(const T item) { save(item, *this); }) {
            NewJsonScope scope(m_serializer);
            save(item, *this);
        } else if constexpr (requires(T item) { item.serialize(*this); }) {
            NewJsonScope scope(m_serializer);
            const_cast<T &>(item).serialize(*this);
        } else if constexpr (requires(T item) { serialize(item, *this); }) {
            NewJsonScope scope(m_serializer);
            serialize(const_cast<T &>(item), *this);
        } else if constexpr (requires(T item) { serialize(item); }) {
            serialize(item);
        } else {
            static_assert(false, "Unsupported type");
        }
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    void serialize(const T &item)
    {
        // auto ptr = reinterpret_cast<const char *>(&item);
        // m_stream.write(ptr, sizeof(T));
        m_serializer(item);
    }

    template <typename T>
        requires std::is_enum_v<T>
    void serialize(const T &item)
    {
        serialize(static_cast<std::underlying_type_t<T>>(item));
    }

    void serialize(const std::string &item) { m_serializer(item); }

    void serialize(const RangeSize &item) { m_serializer(item); }

    template <std::ranges::range Rng>
    void serialize(const Rng &items)
    {
        using T = std::ranges::range_value_t<Rng>;
        std::size_t n;
        if constexpr (std::ranges::sized_range<Rng>) {
            n = std::ranges::size(items);
        } else {
            n = std::distance(std::ranges::begin(items),
                              std::ranges::end(items));
        }
        serialize(RangeSize(n));
        if constexpr (TSerializer::is_binary &&
                      std::ranges::contiguous_range<Rng> &&
                      std::is_arithmetic_v<T>) {
            auto ptr = reinterpret_cast<const char *>(items.data());
            m_stream.write(ptr, n * sizeof(T));
        } else {
            NewJsonScope<true> scope(m_serializer);
            for (const auto &i : items) {
                try_serialize(i);
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
        NewJsonScope scope(m_serializer);
        m_serializer.SetNextName("id");
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
            m_serializer.SetNextName("data");
            try_serialize(*item);
            return;
        } else {
            m_serializer.SetNextName("type_name");
            const auto &type_name = GetClassName(typeid(*item));
            serialize(type_name);
            m_serializer.SetNextName("data");
            const auto &serializer = GetSerializer(type_name);
            serializer(item, *this);
        }
    }
};

class ZENSER_EXPORT InArchive : public ArchiveBase
{
    std::map<void *, std::shared_ptr<void>> m_pointers;
    std::map<std::uintptr_t, void *> m_raw_pointers;

    std::istream &m_stream;

public:
    InArchive(std::istream &stream) : m_stream(stream) {}

    void operator()(auto &&item1, auto &&...items)
    {
        try_serialize(item1);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

private:
    template <typename T>
    void try_serialize(T &item)
    {
        if constexpr (requires(T item) { item.load(*this); }) {
            item.load(*this);
        } else if constexpr (requires(T item) { load(item, *this); }) {
            load(item, *this);
        } else if constexpr (requires(T item) { item.serialize(*this); }) {
            item.serialize(*this);
        } else if constexpr (requires(T item) { serialize(item, *this); }) {
            serialize(item, *this);
        } else if constexpr (requires { serialize(item); }) {
            // the defaul serialization function for basic types
            serialize(item);
        } else {
            static_assert(false,
                          "Unsupported type, define free or member save/load "
                          "function; or serialize function for it.");
        }
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    void serialize(T &item)
    {
        auto ptr = reinterpret_cast<char *>(&item);
        m_stream.read(ptr, sizeof(T));
    }

    template <typename T>
        requires std::is_enum_v<T>
    void serialize(T &item)
    {
        std::underlying_type_t<T> value;
        serialize(value);
        item = static_cast<T>(value);
    }

    template <std::ranges::range Rng>
    void serialize(Rng &items)
    {
        using T = std::ranges::range_value_t<decltype(items)>;
        std::size_t n;
        serialize(n);

        if constexpr (requires { items.resize(n); }) {
            items.resize(n);
        }

        if constexpr (std::ranges::contiguous_range<Rng> &&
                      std::is_arithmetic_v<T>) {
            auto ptr = reinterpret_cast<char *>(items.data());
            m_stream.read(ptr, n * sizeof(T));
        } else if constexpr (requires { typename Rng::mapped_type; }) {
            for (std::size_t i = 0; i < n; ++i) {
                typename std::pair<typename Rng::key_type,
                                   typename Rng::mapped_type>
                    item;
                try_serialize(item);
                items.emplace(std::move(item));
            }
        } else {
            for (auto &i : items) {
                try_serialize(i);
            }
        }
    }

    template <typename T>
    void serialize(std::unique_ptr<T> &item)
    {
        T *ptr;
        serialize(ptr);
        item = std::unique_ptr<T>(ptr);
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
        T *ptr;
        serialize(ptr);
        if (m_pointers.contains(ptr)) {
            item = std::static_pointer_cast<T>(m_pointers[ptr]);
            return;
        }

        item = std::shared_ptr<T>(ptr);
        m_pointers[ptr] = item;
    }

    template <typename T>
        requires std::is_pointer_v<T>
    void serialize(T &ptr)
    {
        std::uintptr_t id;
        serialize(id);
        if (id == 0) {
            ptr = nullptr;
            return;
        }
        if (m_raw_pointers.contains(id)) {
            ptr = static_cast<T>(m_raw_pointers[id]);
            return;
        }

        using TVal = std::remove_pointer_t<T>;

        if constexpr (!std::is_polymorphic_v<TVal>) {
            ptr = Access::Create<TVal>();
            m_raw_pointers[id] = ptr;
            try_serialize(*ptr);
        } else {
            std::string type_name;
            serialize(type_name);
            ptr = Create<TVal>(type_name);
            m_raw_pointers[id] = ptr;
            const auto &deserializer = GetDeserializer(type_name);
            deserializer(ptr, *this);
        }
    }
};

} // namespace zen

/////////// variant serialization ////////////////

#include <variant>
namespace zen
{

template <typename... Args>
void save(const std::variant<Args...> &variant, OutArchive &ar)
{
    ar(variant.index());
    std::visit([&ar](auto &&arg) { ar(arg); }, variant);
}

template <std::size_t N = 0, typename... Args>
std::variant<Args...> create_variant(std::size_t index)
{
    if (index == N) {
        return std::variant<Args...>(std::in_place_index<N>);
    } else {
        if constexpr (N + 1 < sizeof...(Args)) {
            return create_variant<N + 1, Args...>(index);
        } else {
            throw std::out_of_range(std::format("Index out of range {}/{}",
                                                index, sizeof...(Args)));
        }
    }
}

template <typename... Args>
void load(std::variant<Args...> &variant, InArchive &ar)
{
    std::size_t index;
    ar(index);
    variant = create_variant<0, Args...>(index);
    std::visit([&ar](auto &&arg) { ar(arg); }, variant);
}

void save(const std::monostate &, OutArchive &ar) {}

void load(std::monostate &, InArchive &ar) {}
} // namespace zen

//// stack serialization ///////

#include <stack>
namespace zen
{

namespace detail
{
template <typename T>
concept is_stack = requires(T t) {
    t.top();
    t.pop();
    t.push(t.top());
    typename T::container_type;
};

//! Allows access to the protected container in stack
template <is_stack S>
typename S::container_type const &container(const S &stack)
{
    struct H : public S {
        static typename S::container_type const &get(const S &s)
        {
            return s.*(&H::c);
        }
    };

    return H::get(stack);
}
} // namespace detail

template <detail::is_stack S>
void save(const S &s, OutArchive &ar)
{
    ar(detail::container(s));
}

template <detail::is_stack S>
void load(S &s, InArchive &ar)
{
    typename S::container_type c;
    ar(c);
    s = S(std::move(c));
}
} // namespace zen

namespace zen
{
template <typename T1, typename T2>
void save(const std::pair<T1, T2> &pair, OutArchive &ar)
{
    ar(pair.first, pair.second);
}

template <typename T1, typename T2>
void load(std::pair<T1, T2> &pair, InArchive &ar)
{
    ar(pair.first, pair.second);
}

// namespace detail
// {
// template <typename S>
// concept is_set =
//     requires(S s) { s.insert(std::declval<typename S::value_type>()); };
// } // namespace detail

// set deserializer
template <typename T, typename Comp, typename Alloc>
void load(std::set<T, Comp, Alloc> &set, InArchive &ar)
{
    std::size_t n;
    ar(n);
    for (std::size_t i = 0; i < n; ++i) {
        T value;
        ar(value);
        set.insert(std::move(value));
    }
}

} // namespace zen

namespace zen
{
// serialization for tuple

template <typename... Args>
void save(const std::tuple<Args...> &tuple, OutArchive &ar)
{
    std::apply([&](auto &&...elems) { ar(elems...); }, tuple);
}

template <typename... Args>
void load(std::tuple<Args...> &tuple, InArchive &ar)
{
    std::apply([&](auto &&...elems) { ar(elems...); }, tuple);
}
} // namespace zen

namespace zen
{
namespace detail
{
template <typename T>
concept is_bitset = requires(T t) {
    t.to_string();
    t.to_ullong();
    t.flip();
};
} // namespace detail

template <detail::is_bitset T>
void save(const T &t, OutArchive &ar)
{
    ar(t.to_string());
}

template <detail::is_bitset T>
void load(T &t, InArchive &ar)
{
    std::string str;
    ar(str);
    t = T(str);
}
} // namespace zen
