//
// Created by kamiyoru on 2020/11/2.
//

#ifndef KERNEL_FUZZER_DIAG_SERIALIZED_DIAGNOSTICS_FILE_CONVERTER_H
#define KERNEL_FUZZER_DIAG_SERIALIZED_DIAGNOSTICS_FILE_CONVERTER_H

#include "ClangDiagnosticsFile.h"

namespace kfuzz::clang_diag {

    class SerializedDiagnosticsFileConverter : public clang::serialized_diags::SerializedDiagnosticReader {
        ClangDiagnosticsFile &frh;
    public:
        explicit SerializedDiagnosticsFileConverter(ClangDiagnosticsFile &frh) : frh(frh) {}

    protected:
        std::error_code visitCategoryRecord(unsigned id, llvm::StringRef name) override {
            frh.addRecord(new ClangCategoryRecord(id, name));
            return {};
        }

        std::error_code visitDiagFlagRecord(unsigned id, llvm::StringRef name) override {
            frh.addRecord(new ClangDiagFlagRecord(id, name));
            return {};
        }

        std::error_code visitDiagnosticRecord(unsigned severity, const clang::serialized_diags::Location &location,
                                              unsigned category, unsigned flag, llvm::StringRef msg) override {
            frh.addRecord(new ClangDiagnosticRecord(
                    severity, category, flag, location, msg));
            return {};
        }

        std::error_code visitFilenameRecord(unsigned id, unsigned size,
                                            unsigned timestamp,
                                            llvm::StringRef name) override {
            frh.addRecord(new ClangFilenameRecord(
                    id, size, timestamp, name));
            return {};
        }

        std::error_code visitSourceRangeRecord(const clang::serialized_diags::Location &bg,
                                               const clang::serialized_diags::Location &ed) override {
            frh.addRecord(new ClangSourceRangeRecord(bg, ed));
            return {};
        }
    };
}

#endif //KERNEL_FUZZER_DIAG_SERIALIZED_DIAGNOSTICS_FILE_CONVERTER_H
