cmake_minimum_required(VERSION 3.14)
project(BalagurFate3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(editor main.c++ func.c++)
target_include_directories(editor PRIVATE .)
target_link_libraries(editor Threads::Threads)

add_executable(tests test.c++ func.c++)
target_link_libraries(tests GTest::gtest GTest::gtest_main Threads::Threads)
target_include_directories(tests PRIVATE .)

enable_testing()
add_test(NAME editor_tests COMMAND tests)