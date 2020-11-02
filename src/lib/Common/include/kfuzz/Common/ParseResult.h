//
// Created by kamiyoru on 2020/10/21.
//

#ifndef KERNEL_FUZZER_COMMON_PARSE_RESULT_H
#define KERNEL_FUZZER_COMMON_PARSE_RESULT_H


#include <clang/Tooling/Tooling.h>
#include "kfuzz/Common/FPSResult.h"

#include <utility>
#include <cstdio>

namespace kfuzz {

    struct ParsedFunctionInfo {
        std::vector<uint64_t> dependency;
        std::string type;
        std::string name;
        std::string signature;
        std::string body;
        std::string filePath;
        std::string absFilePath;
        uint64_t id;
        uint64_t file_offset;
        uint64_t trait;
        int32_t file_id;

        ParsedFunctionInfo();

        explicit ParsedFunctionInfo(clang::ASTContext &ctx, const FunctionInfo &rhs);
    };

    struct ParsedExternalVarInfo {
        std::vector<uint64_t> dependency;
        std::string type;
        std::string name;
        std::string decl;
        std::string filePath;
        std::string absFilePath;
        uint64_t id;
        uint64_t file_offset;
        int32_t file_id;

        ParsedExternalVarInfo();

        explicit ParsedExternalVarInfo(clang::ASTContext &ctx, const ExternalVarInfo &rhs);
    };

//        type_node->print(llvm::outs(), context.ctx->getPrintingPolicy());

    struct ParsedDeclInfo {
        std::vector<uint64_t> dependency;
        std::string name;
        std::string type;
        std::string signature;
        std::string decl;
        std::string filePath;
        std::string absFilePath;
        uint64_t id;
        uint64_t trait;
        uint64_t file_offset;
        int32_t file_id;

        ParsedDeclInfo();

        ParsedDeclInfo(uint64_t id, std::string decl, uint64_t trait, uint64_t file_offset);

        ParsedDeclInfo(ParsedDeclInfo &&rhs) noexcept;

        ParsedDeclInfo(
                std::string name, std::string type, std::string decl,
                std::string filePath, std::string absFilePath,
                uint64_t id, int32_t file_id, uint64_t file_offset, uint64_t trait
        );

//        static clang::PrintingPolicy printPolicy;

        explicit ParsedDeclInfo(const DeclInfo &rhs);
    };

    struct ParseResult {

        std::map<std::string, std::vector<std::string>> file_inclusion_list;
        std::map<uint64_t, ParsedDeclInfo> type_decls;
        std::vector<ParsedDeclInfo *> serial_type_decls;
        std::map<uint64_t, ParsedFunctionInfo> func_decls;
        std::vector<ParsedFunctionInfo *> serial_func_decls;
        std::map<uint64_t, ParsedExternalVarInfo> var_decls;
        std::vector<ParsedExternalVarInfo *> serial_var_decls;

        ParseResult &getData() {
            return *this;
        }
    };

//    clang::PrintingPolicy ParsedDeclInfo::printPolicy = instantiatePrintPolicy();
}

#endif // KERNEL_FUZZER_COMMON_PARSE_RESULT_H
