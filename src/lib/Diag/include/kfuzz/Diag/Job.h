//
// Created by kamiyoru on 2020/11/2.
//

#ifndef KERNEL_FUZZER_DIAG_JOB_H
#define KERNEL_FUZZER_DIAG_JOB_H

#include "ClangDiagnosticsFile.h"
#include "ClangDiagnosticsResult.h"

namespace kfuzz::clang_diag {
    namespace internal {

        template<typename TStream>
        struct JobWithOutputFile {
            using stream_t = std::remove_all_extents_t<TStream>;

            std::enable_if_t<
                    std::is_same_v<stream_t, llvm::raw_ostream> ||
                    std::is_same_v<stream_t, std::ostream>, stream_t> *
                    ostream;

            explicit JobWithOutputFile(stream_t *ostream) : ostream(ostream) {}
        };

        template<typename Job_>
        struct JobDefFor {
            using type = Job_;
            using func = std::function<std::error_code(Job_ &job)>;

            template<typename ...Args>
            static type create(Args ...args) {
                return std::move(Job_(std::forward<Args>(args)...));
            }
        };
    }

    template<typename TFile, typename TStream>
    struct SerializeRawDiagnosticsJob : public internal::JobWithOutputFile<TStream> {
        IClangDiagnosticsFile <TFile> file;

        SerializeRawDiagnosticsJob(
                IClangDiagnosticsFile <TFile> file,
                typename internal::JobWithOutputFile<TStream>::stream_t *ostream) :
                internal::JobWithOutputFile<TStream>(ostream),
                file(file) {}
    };

    template<typename TStream>
    struct SerializeDiagnosticsJob : public internal::JobWithOutputFile<TStream> {
        ClangDiagnosticsResult result;

        SerializeDiagnosticsJob(
                ClangDiagnosticsResult result,
                typename internal::JobWithOutputFile<TStream>::stream_t *ostream) :
                internal::JobWithOutputFile<TStream>(ostream),
                result(result) {}
    };

    template<typename T>
    using llvm_raw_job = internal::JobDefFor<SerializeRawDiagnosticsJob<T, llvm::raw_ostream>>;
    template<typename T>
    using std_raw_job = internal::JobDefFor<SerializeRawDiagnosticsJob<T, std::ostream>>;

    using llvm_job = internal::JobDefFor<SerializeDiagnosticsJob<llvm::raw_ostream>>;
    using std_job = internal::JobDefFor<SerializeDiagnosticsJob<std::ostream>>;
}
#endif //KERNEL_FUZZER_DIAG_JOB_H
