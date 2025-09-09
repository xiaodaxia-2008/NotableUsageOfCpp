find_package(fmt CONFIG REQUIRED)

link_libraries(fmt::fmt)

add_executable(fmt_derived fmt_derived.cpp)

add_executable(fmt_user_defined_type fmt_user_defined_type.cpp)