#pragma once
#include <spdlog/spdlog.h>

#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

using TByte = char;
using TBuffer = std::vector<char>;

class OutArchive
{
    TBuffer m_buffer;

    std::set<std::uintptr_t> m_pointers;

public:
    OutArchive() {}

    ~OutArchive() { Flush(); }

    void Flush() {}

    const TBuffer &GetBuffer() const { return m_buffer; }

    void Write(std::ostream &os) { os.write(m_buffer.data(), m_buffer.size()); }

    void operator()(auto &&item1, auto &&item2, auto &&...items)
    {
        (*this)(item1);
        (*this)(item2);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    void operator()(const T &item)
    {
        auto ptr = reinterpret_cast<const TByte *>(&item);
        m_buffer.insert(m_buffer.end(), ptr, ptr + sizeof(T));
    }

    void operator()(const std::ranges::range auto &items)
    {
        std::size_t n = items.size();
        (*this)(n);
        for (auto &i : items) {
            (*this)(i);
        }
    }

    template <typename T>
    void operator()(const std::weak_ptr<T> &item)
    {
        // (*this)(reinterpret_cast<std::uintptr_t>(item.lock().get()));
        (*this)(item.lock());
    }

    template <typename T>
    void operator()(const std::shared_ptr<T> &item)
    {
        if (!item) {
            (*this)(std::string_view{typeid(T).name()}, std::uintptr_t(0));
            return;
        }
        std::string type_name = typeid(*item).name();
        auto id = reinterpret_cast<std::uintptr_t>(item.get());
        (*this)(type_name, id);
        if (m_pointers.contains(id)) {
            return;
        }
        m_pointers.insert(id);
        item->serialize(*this);
    }
};

class InArchive
{
    std::map<std::string, std::function<void *()>> constructors;

    std::map<std::uintptr_t, std::shared_ptr<void>> m_pointers;

    TBuffer m_buffer;
    std::size_t m_index = 0;

public:
    InArchive(TBuffer &buffer) : m_buffer(buffer) {}

    void RegisterConstructor(std::string name,
                             std::function<void *()> constructor)
    {
        constructors[std::move(name)] = std::move(constructor);
    }

    template <typename T>
    T *Create(const std::string &name)
    {
        if (constructors.contains(name)) {
            return static_cast<T *>(constructors[name]());
        }

        if constexpr (std::is_default_constructible_v<T> &&
                      !std::is_polymorphic_v<T>) {
            return new T();
        }

        throw std::runtime_error("No constructor for " + name);
    }

    void operator()(auto &&item1, auto &&item2, auto &&...items)
    {
        (*this)(item1);
        (*this)(item2);
        if constexpr (sizeof...(items) > 0) {
            (*this)(items...);
        }
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    void operator()(T &item)
    {
        auto ptr = reinterpret_cast<TByte *>(&item);
        std::copy_n(&m_buffer[m_index], sizeof(T), ptr);
        m_index += sizeof(T);
    }

    void operator()(std::ranges::range auto &items)
    {
        using T = std::ranges::range_value_t<decltype(items)>;
        std::size_t n;
        (*this)(n);
        if constexpr (requires { items.reserve(n); }) {
            items.reserve(n);
        }
        for (std::size_t i = 0; i < n; ++i) {
            T item;
            (*this)(item);
            if constexpr (requires { items.push_back(item); }) {
                items.push_back(std::move(item));
            } else {
                items[i] = std::move(item);
            }
        }
    }

    template <typename T>
    void operator()(std::weak_ptr<T> &item)
    {
        // std::uintptr_t id;
        // (*this)(id);
        // if (m_pointers.contains(id)) {
        //     item = std::static_pointer_cast<T>(m_pointers[id]);
        //     return;
        // } else {
        //     throw std::runtime_error("Invalid pointer id");
        // }
        std::shared_ptr<T> shared;
        (*this)(shared);
        item = shared;
    }

    template <typename T>
    void operator()(std::shared_ptr<T> &item)
    {
        std::string type_name;
        std::uintptr_t id;
        (*this)(type_name, id);
        if (id == 0) {
            return;
        }
        if (m_pointers.contains(id)) {
            item = std::static_pointer_cast<T>(m_pointers[id]);
            return;
        }

        item = std::shared_ptr<T>(Create<T>(type_name));
        m_pointers[id] = item;
        item->serialize(*this);
    }
};