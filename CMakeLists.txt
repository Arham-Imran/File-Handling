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

# google test integration
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

 # For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES "./unit_tests/*.cpp")

add_executable(local_file_tests ${CMAKE_SOURCE_DIR}/unit_tests/small_file/local_file_tests/local_file_tests.cpp)
target_link_libraries(local_file_tests PUBLIC gtest_main local_file)
target_include_directories(local_file_tests PUBLIC ${CMAKE_SOURCE_DIR}/src/small_file/local_file_class)

add_executable(file_manager_tests ${CMAKE_SOURCE_DIR}/unit_tests/large_file/large_file_tests/file_manager_tests.cpp)
target_link_libraries(file_manager_tests PUBLIC gtest_main file_manager)
target_include_directories(file_manager_tests PUBLIC ${CMAKE_SOURCE_DIR}/src/large_file/file_manager_class)

# foreach(TEST_SOURCE ${TEST_SOURCES})  // This doesn't work as include directories aren't linked correctly
#    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
#    add_executable(${TEST_NAME} ${TEST_SOURCE})
#    target_link_libraries(${TEST_NAME} PUBLIC gtest_main file_manager local_file)
#    target_include_directories(${TEST_NAME} 
#    PUBLIC ${CMAKE_SOURCE_DIR}/src/small_file/local_file_class
#    PUBLIC ${CMAKE_SOURCE_DIR}/src/large_file/file_manager_class
#    )
# #  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})   //This is for CTest only. Only one test per executable in ctest
# endforeach()

