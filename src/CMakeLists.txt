# add_subdirectory(qt_examples)
# add_subdirectory(gcode_parser)
# add_subdirectory(pfr_serialization)
# add_subdirectory(ctad)
# add_subdirectory(zpp_serialization)
# add_subdirectory(co_route)
# add_subdirectory(bin_search)
# add_subdirectory(embed_script)
# add_subdirectory(pose_interpolation)
# add_subdirectory(eigen_usage)
# add_subdirectory(stl_examples)
add_subdirectory(cereal_serialization)

# add_executable(cereal_serialization cereal_serialization.cpp)
# target_link_libraries(cereal_serialization PRIVATE cereal::cereal fmt::fmt spdlog::spdlog)

# add_executable(udt_fmt udt_fmt.cpp)
# target_link_libraries(udt_fmt PRIVATE fmt::fmt)

# add_executable(range_remove range_remove.cpp)
# target_link_libraries(range_remove PRIVATE spdlog::spdlog)


add_executable(fmt_derived fmt_derived.cpp)
target_link_libraries(fmt_derived PRIVATE fmt::fmt)