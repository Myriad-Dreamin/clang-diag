
cmake_minimum_required(VERSION 3.15)
project(kernel-fuzzer)

function(add_kfuzz_library library)
    add_library(${library} INTERFACE)

    use_clang_library(${library} ${ARGN})
    use_json_library(${library})
endfunction()

include_directories(.)

add_subdirectory(Common)
add_subdirectory(Diag)

import_fuzz_common_files(FUZZ_COMMON_SOURCE_FILES)

add_subdirectory(Common/test)
