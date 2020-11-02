

# description: import diag source files to array var `files_var_ref`
# param:
#     files_var_ref
macro(import_fuzz_diag_files files_var_ref)
    file(GLOB ${files_var_ref} ${CMAKE_SOURCE_DIR}/src/lib/Diag/**.cpp)
endmacro()

# description: import fps source files to array var `files_var_ref`
# param:
#     files_var_ref
macro(import_fuzz_common_files files_var_ref)
    file(GLOB ${files_var_ref} ${CMAKE_SOURCE_DIR}/src/lib/Common/**.cpp)
endmacro()