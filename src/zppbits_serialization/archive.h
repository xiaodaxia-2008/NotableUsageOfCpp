#pragma once
#include <zpp_bits.h>

#include <rttr/type>
#include <spdlog/spdlog.h>

template <bool IsOutput>
class Archive
{
    using TArchive =
        std::conditional_t<IsOutput, zpp::bits::out<>, zpp::bits::in<>>;

    TArchive ar;

public:
    constexpr Archive(TArchive ar) : ar{std::move(ar)} {}

    constexpr auto operator()(auto &&...items)
    {
        return ar(std::forward<decltype(items)>(items)...);
    }

    template <typename T>
    constexpr auto operator()(std::shared_ptr<T> &item)
    {
        if constexpr (IsOutput) {
            spdlog::info("Serializing {}", typeid(*item).name());
            std::string type_name{rttr::type::get(*item).get_name()};
            spdlog::info("Serializing {}", type_name);
            ar(type_name);
            item->save(*this);
        } else {
            std::string type_name;
            ar(type_name);
            spdlog::info("Deserializing {}", type_name);
            auto type = rttr::type::get_by_name(type_name);
            item = type.create().get_value<std::shared_ptr<T>>();
            item->load(*this);
        }
    }
};

using OutputArchive = Archive<true>;
using InputArchive = Archive<false>;
