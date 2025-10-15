#include "details/binding_map.hpp"
#include "details/static_object.hpp"

#include "archives/binary.hpp"
#include "archives/json.hpp"

namespace cereal
{
namespace detail
{

template <class T>
T &StaticObject<T>::createShared()
{
    static T t;
    return t;
}

template PolymorphicCasters &StaticObject<PolymorphicCasters>::createShared();

template Versions &StaticObject<Versions>::createShared();

template OutputBindingMap<BinaryOutputArchive> &
StaticObject<OutputBindingMap<BinaryOutputArchive>>::createShared();

template InputBindingMap<BinaryInputArchive> &
StaticObject<InputBindingMap<BinaryInputArchive>>::createShared();

template OutputBindingMap<JSONOutputArchive> &
StaticObject<OutputBindingMap<JSONOutputArchive>>::createShared();

template InputBindingMap<JSONInputArchive> &
StaticObject<InputBindingMap<JSONInputArchive>>::createShared();
} // namespace detail
} // namespace cereal