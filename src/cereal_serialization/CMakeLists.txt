include(GenerateExportHeader)

# find_package(cereal CONFIG REQUIRED)

add_subdirectory(cereal_dll/cereal)

link_libraries(spdlog::spdlog cereal_dll)
add_library(dummy_lib_base SHARED Base.cpp)
generate_export_header(dummy_lib_base)

add_library(dummy_lib_derived SHARED Derived.cpp)
generate_export_header(dummy_lib_derived)
target_link_libraries(dummy_lib_derived PUBLIC dummy_lib_base)

add_executable(cereal_serialization main.cpp)
target_link_libraries(cereal_serialization PRIVATE dummy_lib_derived)
