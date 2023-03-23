find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_path(CHAISCRIPT_INCLUDE_DIRS "chaiscript/chaiscript.hpp")

add_executable(sol2_main sol2_main.cpp)
target_include_directories(sol2_main PRIVATE ${CHAISCRIPT_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})
target_link_libraries(sol2_main PRIVATE sol2 ${LUA_LIBRARIES})

add_executable(chai chai.cpp)
target_compile_options(chai PRIVATE /bigobj)
target_include_directories(chai PRIVATE ${CHAISCRIPT_INCLUDE_DIRS})