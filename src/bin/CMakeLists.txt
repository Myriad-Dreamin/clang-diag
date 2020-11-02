
cmake_minimum_required(VERSION 3.15)
project(kernel-fuzzer)

if (UNIX)

    # todo: target_compile_features
    # https://enginetrouble.net/2015/11/trying-to-link-clang-libtooling-into-my-program.html
    # fix: Undefined symbols: clang::tooling::FrontendActionFactory
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
endif ()

# format_diag_main.cpp
add_executable(clang-diag diag_main.cpp)
use_json_library(clang-diag)
target_link_libraries(clang-diag PUBLIC kfuzzCommon kfuzzDiag)
target_compile_definitions(clang-diag PUBLIC CLANG_DIAG_VERSION="1.0.0")