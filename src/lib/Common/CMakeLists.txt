
import_fuzz_common_files(FUZZ_COMMON_SOURCE_FILES)

add_kfuzz_library(
        kfuzzCommon

        ClangLibs
        clangFrontend

        LLVMLibs
        clangFrontend
)
target_sources(kfuzzCommon INTERFACE ${FUZZ_COMMON_SOURCE_FILES})
target_include_directories(kfuzzCommon INTERFACE include)
