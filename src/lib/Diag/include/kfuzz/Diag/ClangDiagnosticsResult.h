//
// Created by kamiyoru on 2020/11/2.
//

#ifndef KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_RESULT_H
#define KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_RESULT_H


#include <kfuzz/Common.h>
#include "ClangDiagnosticsFile.h"

namespace kfuzz::clang_diag {

    struct ExtendedClangDiagnosticRecord : public ClangDiagnosticRecord {
        std::vector<location_t> stack;
        std::vector<ClangDiagnosticRecord> expansion_note;

        explicit ExtendedClangDiagnosticRecord(const ClangDiagnosticRecord &record,
                                               std::vector<location_t> *stack = nullptr) : ClangDiagnosticRecord(
                record) {
            if (stack != nullptr) {
                this->stack.swap(*stack);
            }
        }
    };

    struct ClangDiagnosticsResult {
        std::vector<location_t> stack;
        std::map<category_t, std::string> category_map;
        std::map<flag_t, std::string> flag_map;
        std::map<file_t, std::string> file_map;
        std::vector<ExtendedClangDiagnosticRecord> records;

        template<typename T>
        void parseAppend(IClangDiagnosticsFile <T> file) {
            for (auto record = file.nextRecord(); record != nullptr; record = file.nextRecord()) {
                switch (record->type) {
                    case ClangDiagnosticRecordBase::CategoryRecord:
#define rec ((ClangCategoryRecord*)record)
                        category_map[rec->id] = rec->name;
#undef rec
                        break;
                    case ClangDiagnosticRecordBase::DiagFlagRecord:
#define rec ((ClangDiagFlagRecord*)record)
                        flag_map[rec->id] = rec->name;
#undef rec
                        break;
                    case ClangDiagnosticRecordBase::FilenameRecord:
#define rec ((ClangFilenameRecord*)record)
                        file_map[rec->id] = rec->name;
#undef rec
                        break;
                    case ClangDiagnosticRecordBase::SourceRangeRecord:
                    case ClangDiagnosticRecordBase::Unknown:
                    default:
                        llvm::errs() << "unknown type of res record (" << record->type << ")";
                        break;
                    case ClangDiagnosticRecordBase::DiagnosticRecord:
#define rec ((ClangDiagnosticRecord*)record)
                        auto msg = llvm::StringRef(rec->msg);
                        if (msg.startswith("in file included from")) {
                            stack.push_back(rec->location);
                        } else if (msg.startswith("expanded from macro")) {
                            records.back().expansion_note.emplace_back(*rec);
                        } else {
                            records.emplace_back(*rec, &stack);
                        }
#undef rec
                        break;
                }
            }
        }
    };
}

#endif //KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_RESULT_H
