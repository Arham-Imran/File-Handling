cmake_minimum_required(VERSION 3.5.0)
project(fileManager VERSION 0.1.0 LANGUAGES C CXX)

add_library(fileManager fileManager.cpp)
target_include_directories(fileManager PUBLIC ${CMAKE_SOURCE_DIR}/src/smallFile/localFileClass)
target_link_libraries(fileManager PUBLIC localFile)

