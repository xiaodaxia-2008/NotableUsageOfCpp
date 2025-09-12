include(GenerateExportHeader)

find_package(cereal CONFIG REQUIRED)

link_libraries(cereal::cereal spdlog::spdlog)

add_library(dummy_lib_base SHARED Base.cpp)
generate_export_header(dummy_lib_base)

add_library(dummy_lib_derived SHARED Derived.cpp)
generate_export_header(dummy_lib_derived)
target_link_libraries(dummy_lib_derived PUBLIC dummy_lib_base)

add_executable(cereal_serialization main.cpp)
target_link_libraries(cereal_serialization PRIVATE dummy_lib_derived)
