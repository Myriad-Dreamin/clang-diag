
cmake_minimum_required(VERSION 3.15)
project(kernel-fuzzer)

# Find GTest
if (FUZZER_ENABLE_GTEST)
    if (FUZZER_USE_EXTERNAL_TEST)
    else ()
        if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
            set(LIBRARIES ${LIBRARIES} " " pthread)
        else ()
            message("!!! warning: not using pthread " "(host: " ${CMAKE_SYSTEM_NAME} ")!!!")
        endif ()

        add_subdirectory(googletest)
    endif ()
endif ()

# Find GTest
if (FUZZER_USE_EXTERNAL_JSON)
    find_package(nlohmann_json 3.2.0 REQUIRED)
else ()
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    add_subdirectory(nlohmann_json)
endif ()

# add_subdirectory(fuzzer-extras)
