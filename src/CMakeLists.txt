
cmake_minimum_required(VERSION 3.15)
project(kernel-fuzzer)

# test_main source files -> compile to lib to reduce compile time
add_library(gtest-main STATIC test/test-main.cpp)
target_link_libraries(gtest-main gtest)
target_include_directories(gtest-main SYSTEM PUBLIC gtest)
target_include_directories(gtest-main SYSTEM PUBLIC test)
if (FUZZER_ENABLE_LLVM)
    include_clang_directories(gtest-main)
endif ()

include(lib/MakeFunction.cmake)

add_subdirectory(lib)
add_subdirectory(bin)
