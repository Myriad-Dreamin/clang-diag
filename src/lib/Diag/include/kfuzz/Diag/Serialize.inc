
#ifndef KFUZZ_DIAG_RECORD_JSON_SERIALIZE_SUPPORT
#define KFUZZ_DIAG_RECORD_JSON_SERIALIZE_SUPPORT

#include "kfuzz/Common/ClangDiagnosticRecords.h"
#include "ClangDiagnosticsResult.h"

namespace kfuzz {
    macroDeclareNJSONITmpl(ClangCategoryRecord);

    macroDeclareNJSONITmpl(ClangSourceRangeRecord);

    macroDeclareNJSONITmpl(ClangDiagFlagRecord);

    macroDeclareNJSONITmpl(ClangDiagnosticRecord);

    macroDeclareNJSONITmpl(ClangFilenameRecord);

    macroDeclareNJSONITmpl(location_t);
}
namespace kfuzz {
    macroDeclareNJSONITmpl(clang_diag::ExtendedClangDiagnosticRecord);
}

#endif // KFUZZ_DIAG_RECORD_JSON_SERIALIZE_SUPPORT
