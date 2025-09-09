#include "Base.h"

#include <zpp_bits.h>

#include <rttr/registration>

Base::~Base() {}

void Base::save(OutputArchive &ar) const { ar(i, f, dict); }

void Base::load(InputArchive &ar) { ar(i, f, dict); }

// void Base::serialize(auto &ar, const unsigned int /*version*/)
// {
//     ar(i, f, dict);
// }

// template ZPPBITS_DUMMY_LIB_BASE_EXPORT void
// Base::serialize<zpp::bits::in<std::vector<std::byte>>>(
//     zpp::bits::in<std::vector<std::byte>> &, const unsigned int);

// template ZPPBITS_DUMMY_LIB_BASE_EXPORT void
// Base::serialize<zpp::bits::out<std::vector<std::byte>>>(
//     zpp::bits::out<std::vector<std::byte>> &, const unsigned int);

RTTR_REGISTRATION
{
    rttr::registration::class_<Base>("Base").constructor<>()(
        rttr::policy::ctor::as_std_shared_ptr);
}