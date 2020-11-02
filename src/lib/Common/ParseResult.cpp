
#include <kfuzz/Common/ParseResult.h>
#include <kfuzz/Common/ClangDataTypes.h>

namespace kfuzz {

    inline clang::PrintingPolicy instantiatePrintPolicy() noexcept {
        clang::LangOptions lo;
        clang::PrintingPolicy policy(lo);
        // todo: typedef
        policy.IncludeTagDefinition = true;
        return policy;
    }

    inline const char *getIdentStr(int n) {
        static const char *fast_char_table[] = {
                "ident0", "ident1", "ident2", "ident3", "ident4", "ident5", "ident6", "ident7", "ident8", "ident9",
                "ident10", "ident11", "ident12", "ident13", "ident14",
                "ident15", "ident16", "ident17", "ident18", "ident19"};
        if (n < 20) {
            return fast_char_table[n];
        }
        char *buf = new char[5 + 1 + n];
        snprintf(buf, 5 + 1 + n, "ident%d", n);
        return buf;
    }

    inline void returnIdentStr(int n, const char *buf) {
        if (n < 20) return;
        delete[]buf;
    }

    ParsedDeclInfo::ParsedDeclInfo() :
            id(0), file_id(-1), trait(0), file_offset(0) {}

    ParsedDeclInfo::ParsedDeclInfo(uint64_t id, std::string decl, uint64_t trait, uint64_t file_offset) :
            id(id), file_id(-1), decl(std::move(decl)), trait(trait), file_offset(file_offset) {}

    ParsedDeclInfo::ParsedDeclInfo(ParsedDeclInfo &&rhs) noexcept {
        this->id = rhs.id;
        this->file_id = rhs.file_id;
        this->file_offset = rhs.file_offset;
        this->type = std::move(rhs.type);
        this->decl = std::move(rhs.decl);
        this->name = std::move(rhs.name);
        this->trait = rhs.trait;
        this->absFilePath = std::move(rhs.absFilePath);
        this->filePath = std::move(rhs.filePath);
        this->dependency.swap(rhs.dependency);
    }

    ParsedDeclInfo::ParsedDeclInfo(std::string name, std::string type, std::string decl,
                                   std::string filePath, std::string absFilePath, uint64_t id,
                                   int32_t file_id, uint64_t file_offset, uint64_t trait) :
            name(std::move(name)),
            type(std::move(type)),
            decl(std::move(decl)),
            filePath(std::move(filePath)),
            absFilePath(std::move(absFilePath)),
            id(id),
            file_id(file_id),
            file_offset(file_offset),
            trait(trait) {

    }

    ParsedDeclInfo::ParsedDeclInfo(const DeclInfo &rhs) {
        this->id = rhs.id;
        this->file_id = -1;
        this->file_offset = rhs.file_offset;

        this->type = std::move(clang::QualType(rhs.type, 0).getAsString());
        clang::PrintingPolicy printPolicy = instantiatePrintPolicy();
        this->decl.clear();
        llvm::raw_string_ostream rso(this->decl);
        if (rhs.trait & DeclInfo::IsTypedefEmbeddedAnonymousStruct) {
            rhs.decl->print(rso, printPolicy, 0, true);
        } else {
            rhs.decl->print(rso);
        }
        this->name = std::move(rhs.decl->getNameAsString());
        this->trait = rhs.trait;
        this->absFilePath = rhs.absFilePath.str();
        this->filePath = rhs.filePath.str();
        this->dependency = rhs.dependency;
    }

    ParsedExternalVarInfo::ParsedExternalVarInfo(clang::ASTContext &ctx, const ExternalVarInfo &rhs) {
        this->id = rhs.id;
        this->file_offset = rhs.file_offset;
        this->file_id = -1;

        this->type = std::move(rhs.decl->getType().getAsString());
        this->name = std::move(rhs.decl->getNameAsString());
        this->decl.clear();
        llvm::raw_string_ostream rso(this->decl);
        rhs.decl->print(rso, ctx.getPrintingPolicy());

        this->absFilePath = rhs.absFilePath.str();
        this->filePath = rhs.filePath.str();
        this->dependency = rhs.dependency;
    }

    ParsedExternalVarInfo::ParsedExternalVarInfo() :
            id(0), file_offset(0), file_id(-1) {}

    ParsedFunctionInfo::ParsedFunctionInfo(clang::ASTContext &ctx, const FunctionInfo &rhs) {
        this->id = rhs.id;
        this->file_offset = rhs.file_offset;
        this->trait = rhs.trait;
        this->file_id = -1;

        this->type = std::move(clang::QualType(rhs.type, 0).getAsString());
        this->name = std::move(rhs.decl->getNameAsString());

        auto fd = const_cast<clang::FunctionDecl *>(rhs.decl);
        // todo: not clear all attrs
        fd->dropAttr<clang::Attr>();
        fd->setStorageClass(clang::StorageClass::SC_None);
        int not_used = 0;
        for (auto &p : fd->parameters()) {
            if (p->getName().empty()) {
                auto identName = getIdentStr(not_used);
                auto &the_id = ctx.Idents.getOwn(identName);
                p->setDeclName(clang::DeclarationName(&the_id));
                returnIdentStr(not_used++, identName);
            }
        }
        {
            this->signature.clear();
            llvm::raw_string_ostream rso(this->signature);
            rhs.decl->print(rso, ctx.getPrintingPolicy());
        }
        if (rhs.decl->doesThisDeclarationHaveABody()) {
            this->body.clear();
            llvm::raw_string_ostream rso(this->body);
            // https://github.com/llvm/llvm-project/blob/master/clang/unittests/AST/ASTPrint.h#L31
            rhs.decl->getBody()->printPretty(rso, nullptr, ctx.getPrintingPolicy());

            if (this->signature.size() > this->body.size()) {
                this->signature.erase(this->signature.size() - this->body.size());
            }
        }


        this->absFilePath = rhs.absFilePath.str();
        this->filePath = rhs.filePath.str();
        this->dependency = rhs.dependency;
    }

    ParsedFunctionInfo::ParsedFunctionInfo() :
            id(0), file_offset(0), trait(0), file_id(-1) {}
}