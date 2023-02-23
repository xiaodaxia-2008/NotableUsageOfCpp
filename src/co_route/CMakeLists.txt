set(CMAKE_AUTOMOC ON)
add_executable(co co.cpp CoObj.cpp)
target_link_libraries(co PRIVATE Qt::Widgets spdlog::spdlog)