#pragma once
#include <zpp_bits.h>

#include <spdlog/spdlog.h>

#include <map>
#include <sstream>
#include <variant>

class OutArchive : public zpp::bits::out<>
{
    using TData = std::vector<std::byte>;

    std::map<std::string, std::function<void *()>> constructors;

public:
    using zpp::bits::out<>::out;
    using zpp::bits::out<>::operator();

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
        } else {
            throw std::runtime_error("No constructor for " + name);
        }
    }

    template <typename T>
    zpp::bits::errc serialize_one(const std::weak_ptr<T> &item)
    {
        // (*this)(reinterpret_cast<std::uintptr_t>(item.lock().get()));
        return serialize_one(reinterpret_cast<std::uintptr_t>(item.lock().get()));
    }

    // template <typename T>
    // void operator()(const std::weak_ptr<T> &item)
    // {
    //     (*this)(reinterpret_cast<std::uintptr_t>(item.lock().get()));
    // }

    template <typename T>
    void operator()(const std::shared_ptr<T> &item)
    {
        std::string type_name = typeid(*item).name();
        spdlog::info("Serializing {}", type_name);
        (*this)(type_name);
        item->serialize(*this);
    }
};

class InArchive : public zpp::bits::in<>
{
    using TData = std::vector<std::byte>;

    std::map<std::string, std::function<void *()>> constructors;

public:
    using zpp::bits::in<>::in;
    using zpp::bits::in<>::operator();

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
        } else {
            throw std::runtime_error("No constructor for " + name);
        }
    }

    template <typename T>
    void operator()(std::weak_ptr<T> &item)
    {
        (*this)(reinterpret_cast<std::uintptr_t>(item.lock().get()));
    }

    template <typename T>
    void operator()(std::shared_ptr<T> &item)
    {
        std::string type_name;
        (*this)(type_name);
        spdlog::info("Deserializing {}", type_name);
        item->serialize(*this);
    }
};