add_executable(pfr_serialization pfr_serialization.cpp)
target_link_libraries(pfr_serialization PRIVATE Boost::boost spdlog::spdlog)