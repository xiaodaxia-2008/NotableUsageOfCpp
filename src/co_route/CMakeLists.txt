find_package(concurrencpp CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)
add_executable(co co.cpp CoObj.cpp)
target_link_libraries(co PRIVATE Qt::Widgets spdlog::spdlog)

add_executable(co_gen co_gen.cpp)
target_link_libraries(co_gen PRIVATE spdlog::spdlog)

add_executable(co_task co_task.cpp)
target_link_libraries(co_task PRIVATE concurrencpp::concurrencpp spdlog::spdlog)