
cmake_minimum_required(VERSION 3.15)
project(kernel-fuzzer)

# project declarations
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# configuration
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_definitions(-DYY_NO_UNISTD_H)
endif ()

# options

# options group: enable the targets related to *
option(FUZZER_ENABLE_GTEST "enable gtest related targets" ON)
option(FUZZER_ENABLE_LLVM "enable llvm related targets" ON)
option(FUZZER_ENABLE_CLANG "enable clang related targets" ON)

# options group: use the external library * if the corresponding switch is set to true
option(FUZZER_USE_EXTERNAL_LLVM "use an external LLVM library" ON)
option(FUZZER_USE_EXTERNAL_CLANG "use an external CLANG library" ON)
option(FUZZER_USE_EXTERNAL_TEST "use an external (G)TEST library (default GTEST)" ON)
option(FUZZER_USE_EXTERNAL_JSON "use an external JSON library (default Nlohmann JSON)" OFF)

# use value in config.cmake
include(config.cmake)

message("USING CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})

# include library configurations
include(vendor/LLVM.cmake)
include(vendor/Clang.cmake)
include(vendor/GTest.cmake)
include(vendor/NlohmannJSON.cmake)

# vendor files
add_subdirectory(vendor)

# source files (implementations)
add_subdirectory(src)
