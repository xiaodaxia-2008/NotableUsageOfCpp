add_executable(bin_search bin_search.cpp)
target_link_libraries(bin_search PRIVATE Eigen3::Eigen spdlog::spdlog range-v3::range-v3)