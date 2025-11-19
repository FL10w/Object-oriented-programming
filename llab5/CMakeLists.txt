cmake_minimum_required(VERSION 3.16)
project(lab05)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(CoreLib func.c++)
target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.c++)
target_link_libraries(${PROJECT_NAME} CoreLib)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(${PROJECT_NAME}_tests tests.c++)
target_link_libraries(${PROJECT_NAME}_tests
    CoreLib
    GTest::gtest
    GTest::gtest_main  
)
enable_testing()
add_test(NAME AllTests COMMAND ${PROJECT_NAME}_tests)