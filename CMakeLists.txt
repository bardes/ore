cmake_minimum_required(VERSION 2.8)

project(ore)

set(CMAKE_BUILD_TYPE "Debug")

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
# 
# install(DIRECTORY include DESTINATION . FILES_MATCHING PATTERN *.hpp PATTERN *.h)
# install(DIRECTORY lib DESTINATION .)

add_subdirectory(src)

