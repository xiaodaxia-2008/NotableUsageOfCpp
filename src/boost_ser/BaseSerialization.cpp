#include "Base.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
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


// BOOST_CLASS_EXPORT_GUID(Base, "Base")

void Base::save(std::ostream &oss)
{
    boost::archive::xml_oarchive oa(oss);
    auto sptr = shared_from_this();
    oa << boost::serialization::make_nvp("BaseObject", sptr);
}

std::shared_ptr<Base> Base::load(std::istream &iss)
{
    std::shared_ptr<Base> b;
    boost::archive::xml_iarchive ia(iss);
    ia >> BOOST_SERIALIZATION_NVP(b);
    return b;
}
