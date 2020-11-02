
if (FUZZER_USE_EXTERNAL_JSON)
else ()

    function(use_json_library target)
        target_include_directories(${target} INTERFACE ${CMAKE_SOURCE_DIR}/vendor/nlohmann_json/single_include)
        target_link_libraries(${target} INTERFACE nlohmann_json::nlohmann_json)
    endfunction()
endif ()

