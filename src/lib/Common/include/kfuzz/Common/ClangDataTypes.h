//
// Created by kamiyoru on 2020/10/19.
//

#ifndef KERNEL_FUZZER_COMMON_CLANG_DATA_TYPES_H
#define KERNEL_FUZZER_COMMON_CLANG_DATA_TYPES_H

#include <map>
#include <queue>
#include <utility>
#include <clang/Tooling/Tooling.h>

#include <kfuzz/Common/IncompleteTypeDecls.h>

namespace kfuzz {

    struct Inclusion {
        llvm::StringRef file_name;
        const clang::FileEntry *included_file{};
    };

    struct SymbolInfo {
        std::vector<symbol_hash_t> dependency;
        llvm::StringRef filePath;
        llvm::StringRef absFilePath;
        symbol_hash_t id;
        file_offset_t file_offset;

        explicit SymbolInfo(symbol_hash_t id);

        SymbolInfo(SymbolInfo &&rhs) noexcept;
    };

    struct ExternalVarInfo : public SymbolInfo {
        const clang::VarDecl *decl;
        const clang::Stmt *var;

        explicit ExternalVarInfo(symbol_hash_t id);

        ExternalVarInfo(ExternalVarInfo &&rhs) noexcept;
    };

    struct FunctionInfo : public SymbolInfo {
        using func_char_underlying_t = uint64_t;
        func_char_underlying_t trait;
        const clang::FunctionType *type;
        const clang::FunctionDecl *decl;

        explicit FunctionInfo(symbol_hash_t id);

        FunctionInfo(FunctionInfo &&rhs) noexcept;

        enum FuncChar : func_char_underlying_t {
            NoChar = 0ULL,
            IsDefinition = 1ULL << 1ULL,
            IsUsed = 1ULL << 2ULL,
        };
    };

    struct DeclInfo : public SymbolInfo {
        using decl_char_underlying_t = uint64_t;
        decl_char_underlying_t trait;
        const clang::Type *type = nullptr;
        const clang::NamedDecl *decl = nullptr;

        explicit DeclInfo(symbol_hash_t id);

        DeclInfo(symbol_hash_t id, const clang::Type *type, const clang::NamedDecl *decl, decl_char_underlying_t trait);

        DeclInfo(DeclInfo &&rhs) noexcept;

        DeclInfo(const DeclInfo &rhs) = delete;

        void init(const clang::Type *in_type, const clang::NamedDecl *in_decl, decl_char_underlying_t in_trait);

        enum DeclChar : decl_char_underlying_t {
            NoChar = 0ULL,
            IsDefinition = 1ULL << 1ULL,

            /// to work around the following situation:
            ///
            /// ```code
            /// type struct { int b; } a;
            /// ```
            IsTypedefEmbeddedAnonymousStruct = 1ULL << 2ULL,

            /// to work around the following situation:
            ///
            /// // a_t.h
            /// ```code
            /// type struct A* a_t;
            /// ```
            ///
            /// // a.h
            /// ```code
            /// struct A { a_t a; };
            /// ```
            ///
            /// todo: FIX ME: however there exists the other situation:
            /// typedef void (*f_t) (struct depA*, struct depB*)
            /// struct depA { f_t f };
            /// struct depB { f_t f };
            IsTypedefUsingIncompleteType = 1ULL << 3ULL,

            IsEmbeddedDecl = 1ULL << 4ULL,
        };
    };
}

#endif //KERNEL_FUZZER_COMMON_CLANG_DATA_TYPES_H
