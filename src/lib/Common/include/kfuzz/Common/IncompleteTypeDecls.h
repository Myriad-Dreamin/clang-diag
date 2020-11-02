//
// Created by kamiyoru on 2020/10/29.
//

#ifndef KERNEL_FUZZER_COMMON_INCOMPLETE_TYPE_DECLS_H
#define KERNEL_FUZZER_COMMON_INCOMPLETE_TYPE_DECLS_H

#include <map>

/// Logger.h type index
namespace kfuzz {
    class Logger;

    class StreamLogger;
}

/// ClangDataTypes.h type index
namespace kfuzz {

    using symbol_hash_t = uint64_t;
    using file_offset_t = uint64_t;

    struct SymbolInfo;
    struct ExternalVarInfo;
    struct FunctionInfo;
    struct DeclInfo;
    struct Inclusion;

    using type_decl_map = std::map<symbol_hash_t, DeclInfo *>;
    using func_decl_map = std::map<symbol_hash_t, FunctionInfo *>;
}

/// FPS Exported Common type index
namespace kfuzz {
    class FPSResult;

    class ParseResult;
}

#endif //KERNEL_FUZZER_COMMON_INCOMPLETE_TYPE_DECLS_H
