#include "Derived.h"

#include <zpp_bits.h>

Derived::~Derived() {}

// void Derived::serialize(auto &ar, const unsigned int)
// {
//     ar(static_cast<Base &>(*this), queue);
// }

// template ZPPBITS_DUMMY_LIB_DERIVED_EXPORT void
// Derived::serialize<zpp::bits::in<std::vector<std::byte>>>(
//     zpp::bits::in<std::vector<std::byte>> &ar, const unsigned int);

// template ZPPBITS_DUMMY_LIB_DERIVED_EXPORT void
// Derived::serialize<zpp::bits::out<std::vector<std::byte>>>(
//     zpp::bits::out<std::vector<std::byte>> &ar, const unsigned int);