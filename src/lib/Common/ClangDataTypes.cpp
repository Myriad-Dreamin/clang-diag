//
// Created by kamiyoru on 2020/10/24.
//

#include "kfuzz/Common/ClangDataTypes.h"

namespace kfuzz {

    SymbolInfo::SymbolInfo(uint64_t id) : id(id), file_offset(0) {}

    SymbolInfo::SymbolInfo(SymbolInfo &&rhs) noexcept {
        this->id = rhs.id;
        this->file_offset = rhs.file_offset;
        this->absFilePath = rhs.absFilePath;
        this->filePath = rhs.filePath;
        this->dependency.swap(rhs.dependency);
    }

    DeclInfo::DeclInfo(uint64_t id)
            : SymbolInfo(id), type(nullptr), decl(nullptr), trait(NoChar) {}

    DeclInfo::DeclInfo(uint64_t id, const clang::Type *type, const clang::NamedDecl *decl, decl_char_underlying_t trait)
            : SymbolInfo(id), type(type), decl(decl), trait(trait) {}

    DeclInfo::DeclInfo(DeclInfo &&rhs) noexcept:
            SymbolInfo(std::move(rhs)) {
        this->type = rhs.type;
        rhs.type = nullptr;
        this->decl = rhs.decl;
        rhs.decl = nullptr;
        this->trait = rhs.trait;
        rhs.trait = NoChar;
    }

    void DeclInfo::init(const clang::Type *in_type, const clang::NamedDecl *in_decl, decl_char_underlying_t in_trait) {
        if (nullptr != this->type) {
            if (this->type != in_type) {
                assert(in_type != nullptr);
                this->type->dump();
                in_type->dump();
            }
        }
        this->type = in_type;
        if (nullptr != this->decl) {
            if (this->decl != in_decl) {
                assert(in_decl != nullptr);
                this->decl->dump();
                in_decl->dump();
            }
        }
        this->decl = in_decl;
        this->trait |= in_trait;
    }

    FunctionInfo::FunctionInfo(uint64_t id) : SymbolInfo(id), trait(NoChar) {
        type = nullptr;
        decl = nullptr;
    }

    FunctionInfo::FunctionInfo(FunctionInfo &&rhs) noexcept:
            SymbolInfo(std::move(rhs)) {
        this->type = rhs.type;
        rhs.type = nullptr;
        this->decl = rhs.decl;
        rhs.decl = nullptr;
        this->trait = rhs.trait;
        rhs.trait = NoChar;
    }

    ExternalVarInfo::ExternalVarInfo(uint64_t id) : SymbolInfo(id) {
        var = nullptr;
        decl = nullptr;
    }

    ExternalVarInfo::ExternalVarInfo(ExternalVarInfo &&rhs) noexcept:
            SymbolInfo(std::move(rhs)) {
        this->var = rhs.var;
        rhs.var = nullptr;
        this->decl = rhs.decl;
        rhs.decl = nullptr;
    }
}
