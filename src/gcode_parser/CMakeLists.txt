add_executable(gcode_parese gcode_parser.cpp)
target_link_libraries(gcode_parese PRIVATE range-v3::range-v3 spdlog::spdlog Boost::boost)