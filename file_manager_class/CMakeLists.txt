cmake_minimum_required(VERSION 3.5.0)
project(file_manager VERSION 0.1.0 LANGUAGES C CXX)

add_library(file_manager file_manager.cpp)
target_include_directories(file_manager PUBLIC ${CMAKE_SOURCE_DIR}/local_file_class)
target_link_libraries(file_manager PUBLIC local_file)

