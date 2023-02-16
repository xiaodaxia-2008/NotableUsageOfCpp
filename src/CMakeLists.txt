add_subdirectory(qt_examples)
add_subdirectory(gcode_parser)
add_subdirectory(pfr_serialization)
add_subdirectory(ctad)

add_executable(eigen_fmt eigen_fmt.cpp)
target_link_libraries(eigen_fmt PRIVATE Eigen3::Eigen fmt::fmt spdlog::spdlog)

add_executable(cereal_serialization cereal_serialization.cpp)
target_link_libraries(cereal_serialization PRIVATE cereal::cereal fmt::fmt spdlog::spdlog)

add_executable(udt_fmt udt_fmt.cpp)
target_link_libraries(udt_fmt PRIVATE fmt::fmt)

add_executable(range_remove range_remove.cpp)
target_link_libraries(range_remove PRIVATE spdlog::spdlog)

