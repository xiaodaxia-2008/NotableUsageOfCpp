#include "Base.h"

#include <boost/serialization/deque.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/queue.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/weak_ptr.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


// BOOST_CLASS_EXPORT_GUID(Base, "Base")

void Base::save(auto &ar)
{
    auto sptr = shared_from_this();
    ar &boost::serialization::make_nvp("BaseObject", sptr);
}

std::shared_ptr<Base> Base::load(auto &ar)
{
    std::shared_ptr<Base> b;
    ar &boost::serialization::make_nvp("BaseObject", b);
    return b;
}

template DUMMY_BASE_EXPORT void
Base::save<boost::archive::text_oarchive>(boost::archive::text_oarchive &);
template DUMMY_BASE_EXPORT void
Base::save<boost::archive::binary_oarchive>(boost::archive::binary_oarchive &);
template DUMMY_BASE_EXPORT void
Base::save<boost::archive::xml_oarchive>(boost::archive::xml_oarchive &);

template DUMMY_BASE_EXPORT std::shared_ptr<Base>
Base::load<boost::archive::text_iarchive>(boost::archive::text_iarchive &);
template DUMMY_BASE_EXPORT std::shared_ptr<Base>
Base::load<boost::archive::binary_iarchive>(boost::archive::binary_iarchive &);
template DUMMY_BASE_EXPORT std::shared_ptr<Base>
Base::load<boost::archive::xml_iarchive>(boost::archive::xml_iarchive &);