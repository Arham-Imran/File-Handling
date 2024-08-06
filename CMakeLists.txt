cmake_minimum_required(VERSION 3.5.0)
project(File_Handling VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src/small_file/local_file_class)
add_subdirectory(src/large_file/file_manager_class)

target_link_libraries(${PROJECT_NAME} PUBLIC file_manager)
target_include_directories(${PROJECT_NAME} 
    PUBLIC src/small_file/local_file_class
    PUBLIC src/large_file/file_manager_class
)

