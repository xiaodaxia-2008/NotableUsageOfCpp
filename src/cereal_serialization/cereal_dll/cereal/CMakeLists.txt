add_library(cereal_dll SHARED cereal.cpp)

target_compile_definitions(cereal_dll PUBLIC
    _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING
)

target_include_directories(cereal_dll PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)