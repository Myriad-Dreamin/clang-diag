//
// Created by kamiyoru on 2020/10/17.
//

#ifndef KERNEL_FUZZER_HELPER_H
#define KERNEL_FUZZER_HELPER_H

#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Error.h"
#include "clang/AST/Mangle.h"

// helper functions
namespace kfuzz {

    const clang::Type *getInnerType(const clang::Type *t);

    clang::QualType getInnerQualType(clang::QualType t);

    clang::NamedDecl *getDecl(const clang::Type *underlying);

    template<typename FuzzerInsertingType>
    clang::NamedDecl *getDecl(const clang::Type *underlying, bool &isDefinition);

    template<typename FuzzerInsertingType>
    clang::NamedDecl *getDecl(const clang::Type *underlying);

    llvm::StringRef getIncludePath(const clang::SourceManager &sm, clang::SourceLocation loc);

    void handleLLVMError(llvm::Error err);

    uint64_t createTypeMangleDigest(clang::MangleContext &mangle_ctx, const clang::Type *t);

    uint64_t
    createVariableMangleDigest(clang::MangleContext &mangle_ctx, const clang::Type *t, llvm::StringRef varName);

    uint64_t createFunctionMangleDigest(
            clang::MangleContext &mangle_ctx, const clang::FunctionType *t, llvm::StringRef funcName);
}

#endif //KERNEL_FUZZER_HELPER_H
