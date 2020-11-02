
if (FUZZER_ENABLE_GTEST)

    # description: create a gtest target
    # param:
    #     test_name: string, the name of the target (also the name of the entry file)
    # example:
    #     if there is a test source file named my_test
    #     + my_test.cpp (without main function which has been provided by test/test_main.cpp)
    #     then you should add following target to CMakeList.txt
    #     + add_gtest(my_test)
    function(add_gtest test_name)
        add_executable(${test_name} ${test_name}.cpp ${ARGN})

        target_link_libraries(${test_name} PUBLIC gtest-main)
        if (MINGW)
            target_link_libraries(${test_name} PUBLIC -static-libgcc -static-libstdc++)
        endif ()
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

endif ()
