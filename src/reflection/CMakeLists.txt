find_package(spdlog CONFIG REQUIRED)
link_libraries(spdlog::spdlog)

add_library(reflection reflection.cpp)

add_executable(reflection_basic main.cpp)
target_link_libraries(reflection_basic PRIVATE reflection)