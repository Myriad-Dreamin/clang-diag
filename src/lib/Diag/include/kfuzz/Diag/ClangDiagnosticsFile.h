//
// Created by kamiyoru on 2020/11/2.
//

#ifndef KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_FILE_H
#define KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_FILE_H


#include <kfuzz/Common.h>

namespace kfuzz::clang_diag {


#define macroLetClassRef_(ClassName, T) \
    private:\
    T *ref;\
    public:\
    explicit ClassName(T &ref) : ref(&ref) {}\
    explicit ClassName(T *ref) : ref(*ref) {}\
    private:

#define macroLetClassRef(ClassName) macroLetClassRef_(ClassName, T)

    template<typename T>
    class IClangDiagnosticsFile {
    macroLetClassRef(IClangDiagnosticsFile)
    public:

        ClangDiagnosticRecordBase *nextRecord() {
            return ref->nextRecord();
        }

        void reset() {
            ref->reset();
        }
    };

    class ClangDiagnosticsFile {
        std::vector<ClangDiagnosticRecordBase *> records;
        int64_t ptr = 0;
    public:
        ClangDiagnosticRecordBase *nextRecord() {
            if (ptr >= records.size()) {
                return nullptr;
            }
            return records[ptr++];
        }

        ~ClangDiagnosticsFile() {
            for (auto &record: records) {
                delete record;
                record = nullptr;
            }
        }

        void reset() {
            ptr = 0;
        }

        size_t size() {
            return records.size();
        }

        ClangDiagnosticRecordBase &operator[](int index) {
            return *records[index];
        }

        void addRecord(ClangDiagnosticRecordBase *record) {
            records.push_back(record);
        }
    };
}

#endif //KERNEL_FUZZER_DIAG_CLANG_DIAGNOSTICS_FILE_H
