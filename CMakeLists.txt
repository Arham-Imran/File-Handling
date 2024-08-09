cmake_minimum_required(VERSION 3.5.0)
project(File_Handling VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src/smallFile/localFileClass)
add_subdirectory(src/largeFile/fileManagerClass)

target_link_libraries(${PROJECT_NAME} PUBLIC fileManager)
target_include_directories(${PROJECT_NAME} 
    PUBLIC src/smallFile/localFileClass
    PUBLIC src/largeFile/fileManagerClass
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

# file(GLOB_RECURSE TEST_SOURCES "./unitTests/*.cpp")

add_executable(localFileTests ${CMAKE_SOURCE_DIR}/unitTests/smallFile/localFileTests/localFileTests.cpp)
target_link_libraries(localFileTests PUBLIC gtest_main localFile)
target_include_directories(localFileTests PUBLIC ${CMAKE_SOURCE_DIR}/src/smallFile/localFileClass)

add_executable(fileManagerTests ${CMAKE_SOURCE_DIR}/unitTests/largeFile/largeFileTests/fileManagerTests.cpp)
target_link_libraries(fileManagerTests PUBLIC gtest_main fileManager)
target_include_directories(fileManagerTests PUBLIC ${CMAKE_SOURCE_DIR}/src/largeFile/fileManagerClass)

# foreach(TEST_SOURCE ${TEST_SOURCES})  // This doesn't work as include directories aren't linked correctly
#    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
#    add_executable(${TEST_NAME} ${TEST_SOURCE})
#    target_link_libraries(${TEST_NAME} PUBLIC gtest_main fileManager localFile)
#    target_include_directories(${TEST_NAME} 
#    PUBLIC ${CMAKE_SOURCE_DIR}/src/smallFile/localFileClass
#    PUBLIC ${CMAKE_SOURCE_DIR}/src/largeFile/fileManagerClass
#    )
# #  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})   //This is for CTest only. Only one test per executable in ctest
# endforeach()

