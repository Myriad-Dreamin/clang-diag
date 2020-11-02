//
// Created by kamiyoru on 2020/11/2.
//

#include <kfuzz/Common/ClangDiagnosticRecords.h>

namespace kfuzz {
    ClangDiagnosticRecordBase::ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::Type type) : type(type) {}

    kfuzz::ClangCategoryRecord::ClangCategoryRecord(category_t id, llvm::StringRef name) :
            ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::CategoryRecord), id(id), name(name) {
    }

    ClangDiagFlagRecord::ClangDiagFlagRecord(flag_t id, llvm::StringRef name) :
            ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::DiagFlagRecord), id(id), name(name) {
    }

    ClangDiagnosticRecord::ClangDiagnosticRecord(severity_t severity, category_t category, flag_t flag,
                                                 location_t location, llvm::StringRef msg) :
            ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::DiagnosticRecord),
            severity(severity), category(category), flag(flag), location(location), msg(msg) {
    }

    ClangFilenameRecord::ClangFilenameRecord(file_t id, size_t size, timestamp_t timestamp, llvm::StringRef name) :
            ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::FilenameRecord),
            id(id), size(size), timestamp(timestamp), name(name) {
    }

    ClangSourceRangeRecord::ClangSourceRangeRecord(location_t begin_loc, location_t end_loc) :
            ClangDiagnosticRecordBase(ClangDiagnosticRecordBase::SourceRangeRecord),
            begin_loc(begin_loc), end_loc(end_loc) {}
}
