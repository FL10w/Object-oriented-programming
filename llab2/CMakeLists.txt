cmake_minimum_required(VERSION 3.16)
project(llab2)
set(CMAKE_CXX_STANDARD 20)
#Установка Google Test
include(FetchContent)
FetchContent_Declare(
googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG v1.15.0
TLS_VERIFY false
)
find_package(GTest REQUIRED)
add_library(${CMAKE_PROJECT_NAME}_lib func.c++)
add_executable(${CMAKE_PROJECT_NAME}_exe main.c++)
target_link_libraries(${CMAKE_PROJECT_NAME}_exe ${CMAKE_PROJECT_NAME}_lib)
add_executable(tests tests.c++)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib GTest::GTest GTest::Main)
enable_testing()
add_test(NAME NumberExtractorTests COMMAND tests)