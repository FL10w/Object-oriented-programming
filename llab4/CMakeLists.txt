cmake_minimum_required(VERSION 3.16)
project(lab04_metaprogramming)
set(CMAKE_CXX_STANDARD 20)
add_library(GeometryLib Geometry.c++)
add_executable(${PROJECT_NAME} main.c++)
target_link_libraries(${PROJECT_NAME} GeometryLib)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
FetchContent_MakeAvailable(googletest)
add_executable(GeometryTests tests.c++)
target_link_libraries(GeometryTests GeometryLib GTest::gtest GTest::gtest_main)
enable_testing()
add_test(NAME GeometryTests COMMAND GeometryTests)