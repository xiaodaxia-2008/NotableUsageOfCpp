include(GenerateExportHeader)

find_package(Bitsery CONFIG REQUIRED)

link_libraries(Bitsery::bitsery spdlog::spdlog)

add_library(bitsery_lib_base SHARED Base.cpp)
generate_export_header(bitsery_lib_base)

add_library(bitsery_lib_derived SHARED Derived.cpp)
generate_export_header(bitsery_lib_derived)
target_link_libraries(bitsery_lib_derived PUBLIC bitsery_lib_base)

add_executable(scene_bitsery main.cpp)
target_link_libraries(scene_bitsery PRIVATE bitsery_lib_derived)
