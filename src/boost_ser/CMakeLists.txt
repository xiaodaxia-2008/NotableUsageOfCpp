include(GenerateExportHeader)

find_package(Boost REQUIRED Serialization)

add_library(dummy_base SHARED Base.cpp BaseSerialization.cpp)
generate_export_header(dummy_base)
target_link_libraries(dummy_base PUBLIC spdlog::spdlog fmt::fmt Boost::Serialization)

add_library(dummy_derived SHARED Derived.cpp DerivedSerialization.cpp)
generate_export_header(dummy_derived)
target_link_libraries(dummy_derived PUBLIC dummy_base)

# add_library(dummy_ser3 SHARED Serialization.cpp)
# target_link_libraries(dummy_ser3 PUBLIC spdlog::spdlog fmt::fmt Boost::Serialization)
add_executable(boost_ser boost_ser.cpp)
target_link_libraries(boost_ser PRIVATE dummy_derived)
