include(GenerateExportHeader)

# find_path(ZPP_BITS_INCLUDE_DIRS "zpp_bits.h")
# include_directories(${ZPP_BITS_INCLUDE_DIRS})
add_library(zen_serialization SHARED Archive.cpp)
generate_export_header(zen_serialization)

add_library(zppbits_dummy_lib_base SHARED Base.cpp)
generate_export_header(zppbits_dummy_lib_base)
target_link_libraries(zppbits_dummy_lib_base PUBLIC zen_serialization)

add_library(zppbits_dummy_lib_derived SHARED Derived.cpp)
generate_export_header(zppbits_dummy_lib_derived)
target_link_libraries(zppbits_dummy_lib_derived PUBLIC zppbits_dummy_lib_base)

add_executable(scene_zppbits main.cpp)
target_link_libraries(scene_zppbits PRIVATE zppbits_dummy_lib_derived)

add_executable(zppbits_basic basic_example.cpp)