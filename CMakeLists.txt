cmake_minimum_required(VERSION 3.10)

project(record_test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(GoogleTest)

find_package(GTest REQUIRED)
add_executable(record_test "test.cpp")
target_link_libraries(record_test PRIVATE GTest::gtest_main)
gtest_discover_tests(record_test)