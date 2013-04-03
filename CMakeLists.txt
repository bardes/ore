# Basic info
cmake_minimum_required(VERSION 2.8)
project(ore)
set(CMAKE_BUILD_TYPE "Debug")
#set(CPACK_PACKAGE_VERSION "0.1.0")

# Specifying the lib and include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(src)

