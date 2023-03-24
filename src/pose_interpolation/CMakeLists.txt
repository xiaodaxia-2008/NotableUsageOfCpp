find_package(Eigen3 CONFIG REQUIRED)
add_executable(pose_interpolation pose_interpolation.cpp)
target_link_libraries(pose_interpolation PRIVATE Eigen3::Eigen spdlog::spdlog)