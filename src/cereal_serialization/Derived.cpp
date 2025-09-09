#include "Derived.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/memory.hpp>

Derived::~Derived() {}

void Derived::serialize(auto &ar, const unsigned int)
{
    ar(cereal::base_class<Base>(this), queue);
}

template DUMMY_LIB_DERIVED_EXPORT void
Derived::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive &ar,
                                              const unsigned int);

template DUMMY_LIB_DERIVED_EXPORT void
Derived::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive &ar,
                                                const unsigned int);

template DUMMY_LIB_DERIVED_EXPORT void
Derived::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive &ar,
                                             const unsigned int);

template DUMMY_LIB_DERIVED_EXPORT void
Derived::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive &ar,
                                               const unsigned int);