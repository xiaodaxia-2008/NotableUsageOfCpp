#include "Base.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>

Base::~Base() {}

void Base::serialize(auto &ar, const unsigned int /*version*/)
{
    ar(i, f, dict);
}

template DUMMY_LIB_BASE_EXPORT void
Base::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive &,
                                           const unsigned int);

template DUMMY_LIB_BASE_EXPORT void
Base::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive &,
                                             const unsigned int);

template DUMMY_LIB_BASE_EXPORT void
Base::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive &,
                                          const unsigned int);

template DUMMY_LIB_BASE_EXPORT void
Base::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive &,
                                            const unsigned int);