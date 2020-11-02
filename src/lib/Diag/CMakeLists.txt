
import_fuzz_diag_files(FUZZ_CLANG_DIAG_SOURCE_FILES)

add_kfuzz_library(
        kfuzzDiag

        ClangLibs
        clangFrontend

        LLVMLibs
        clangFrontend
)
target_sources(kfuzzDiag INTERFACE ${FUZZ_CLANG_DIAG_SOURCE_FILES})
target_link_libraries(kfuzzDiag INTERFACE kfuzzCommon)
target_include_directories(kfuzzDiag INTERFACE include)
