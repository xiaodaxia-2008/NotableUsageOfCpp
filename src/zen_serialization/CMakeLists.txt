include(GenerateExportHeader)
find_package(spdlog CONFIG REQUIRED)
link_libraries(spdlog::spdlog)

add_library(zenser SHARED Archive.cpp)
generate_export_header(zenser)

add_library(zenser_dummy_lib_base SHARED Base.cpp)
generate_export_header(zenser_dummy_lib_base)
target_link_libraries(zenser_dummy_lib_base PUBLIC zenser)

add_library(zenser_dummy_lib_derived SHARED Derived.cpp)
generate_export_header(zenser_dummy_lib_derived)
target_link_libraries(zenser_dummy_lib_derived PUBLIC zenser_dummy_lib_base)

add_executable(scene_zen_ser main.cpp)
target_link_libraries(scene_zen_ser PRIVATE zenser_dummy_lib_derived)