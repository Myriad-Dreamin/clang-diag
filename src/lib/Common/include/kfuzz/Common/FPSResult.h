//
// Created by kamiyoru on 2020/10/30.
//

#ifndef KERNEL_FUZZER_COMMON_FPS_RESULT_H
#define KERNEL_FUZZER_COMMON_FPS_RESULT_H

#include <clang/Tooling/Tooling.h>
#include <kfuzz/Common/IncompleteTypeDecls.h>

namespace kfuzz {

    struct FPSResult {

        virtual void handleDeclInfo(clang::ASTContext &ctx, const DeclInfo &info) {}

        virtual void handleFunctionInfo(clang::ASTContext &context, const FunctionInfo &second) {}

        virtual void handleExternalVarInfo(clang::ASTContext &context, const ExternalVarInfo &second) {}

        virtual void handleSourceInclusion(
                clang::ASTContext &ctx,
                llvm::Twine including_file,
                llvm::Twine included_file) {}

        void release() {}
    };
}

#endif