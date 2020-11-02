//
// Created by kamiyoru on 2020/11/2.
//

#ifndef KERNEL_FUZZER_COMMON_CLANG_DIAGNOSTIC_RECORDS_H
#define KERNEL_FUZZER_COMMON_CLANG_DIAGNOSTIC_RECORDS_H

#include <clang/Frontend/SerializedDiagnosticReader.h>

namespace kfuzz {

    using file_t = unsigned;
    using severity_t = unsigned;
    using category_t = unsigned;
    using size_t = unsigned;
    using timestamp_t = unsigned;
    using flag_t = unsigned;
    using location_t = clang::serialized_diags::Location;

    struct ClangDiagnosticRecordBase {
        enum Type : uint8_t {
            Unknown,
            CategoryRecord,
            DiagFlagRecord,
            DiagnosticRecord,
            FilenameRecord,
            SourceRangeRecord,
        } type;

        explicit ClangDiagnosticRecordBase(Type type);
    };

    struct ClangCategoryRecord : ClangDiagnosticRecordBase {
        category_t id;
        std::string name;


        ClangCategoryRecord(category_t id, llvm::StringRef name);
    };

    struct ClangDiagFlagRecord : ClangDiagnosticRecordBase {
        flag_t id;
        std::string name;

        ClangDiagFlagRecord(flag_t id, llvm::StringRef name);
    };

    struct ClangDiagnosticRecord : ClangDiagnosticRecordBase {
        severity_t severity{};
        category_t category{};
        flag_t flag{};
        location_t location{0, 0, 0, 0};
        std::string msg;

        ClangDiagnosticRecord(
                severity_t severity, category_t category, flag_t flag,
                location_t location, llvm::StringRef msg);

        ClangDiagnosticRecord(const ClangDiagnosticRecord &record) = default;
    };

    struct ClangFilenameRecord : ClangDiagnosticRecordBase {
        file_t id;
        size_t size{};
        timestamp_t timestamp{};
        std::string name;

        ClangFilenameRecord(file_t id, size_t size, timestamp_t timestamp, llvm::StringRef name);
    };

    struct ClangSourceRangeRecord : ClangDiagnosticRecordBase {
        location_t begin_loc{0, 0, 0, 0};
        location_t end_loc{0, 0, 0, 0};

        ClangSourceRangeRecord(location_t begin_loc, location_t end_loc);
    };
}

#endif //KERNEL_FUZZER_COMMON_CLANG_DIAGNOSTIC_RECORDS_H
