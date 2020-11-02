
#ifndef KFUZZ_FPS_PARSE_RESULT_JSON_SERIALIZE_SUPPORT
#define KFUZZ_FPS_PARSE_RESULT_JSON_SERIALIZE_SUPPORT

#include "kfuzz/Common/ParseResult.h"

namespace kfuzz {

    macroDeclareNJSONITmpl(ParseResult);

    macroDeclareNJSONITmpl(ParsedDeclInfo);

    macroDeclareNJSONITmpl(ParsedFunctionInfo);

    macroDeclareNJSONITmpl(ParsedExternalVarInfo);
}

#endif // KFUZZ_FPS_PARSE_RESULT_JSON_SERIALIZE_SUPPORT
