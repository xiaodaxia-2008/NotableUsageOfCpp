# Copyright © 2022 Zen Shawn. All rights reserved.
# Created at 14:53:49, December 21, 2022

cmake_minimum_required(VERSION 3.13)

option(BUILD_SHARED_LIBS "build shared libs" OFF)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "build type, Release/Debug/MinSizeRel/RelWithDebInfo")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(Temp VERSION 0.0.0 DESCRIPTION "Instroduction about Temp")

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  message(STATUS "Found ccache")
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/Programs/Temp" CACHE PATH "install prefix")

find_package(range-v3 CONFIG)
find_package(spdlog CONFIG)
find_package(fmt CONFIG)
find_package(Boost)
find_package(Qt6 CONFIG COMPONENTS Widgets)
find_package(cereal CONFIG)
find_package(Eigen3 CONFIG)

add_subdirectory(src)