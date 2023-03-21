find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)

add_executable(sol2_main sol2_main.cpp)
target_include_directories(sol2_main PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(sol2_main PRIVATE sol2 ${LUA_LIBRARIES})