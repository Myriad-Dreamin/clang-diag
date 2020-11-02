//
// Created by kamiyoru on 2020/10/17.
//

#include <llvm/Support/FormatVariadic.h>
#include <kfuzz/helper.h>

namespace kfuzz {

    const clang::Type *getInnerType(const clang::Type *t) {
        if (t == nullptr) {
            return t;
        }
        if (llvm::isa<clang::TypedefType>(t)) {
            return t;
        }
        if (t->isPointerType() || t->isArrayType()) {
            return getInnerType(t->getPointeeOrArrayElementType());
        }
        if (auto pt = llvm::dyn_cast<clang::ParenType>(t)) {
            return getInnerType(pt->getInnerType().getTypePtr());
        }
        if (t->isBuiltinType()) {
            return t;
        }
        if (auto et = llvm::dyn_cast<clang::ElaboratedType>(t)) {
            return getInnerType(et->getUnqualifiedDesugaredType());
        }
        return t;
    }

    clang::QualType getInnerQualType(clang::QualType t) {
        return clang::QualType(getInnerType(t.getTypePtr()), 0);
    }

    llvm::StringRef getIncludePath(const clang::SourceManager &sm, clang::SourceLocation loc) {
        llvm::StringRef FilePath;
        while (loc.isValid() && !sm.isInMainFile(loc)) {
            FilePath = sm.getFilename(loc);
            if (FilePath.empty() || !FilePath.endswith(".inc"))
                break;
            loc = sm.getIncludeLoc(sm.getFileID(loc));
        }
        return FilePath;
    }

    void handleLLVMError(llvm::Error err) {
        if (err) {
            std::string str;
            llvm::raw_string_ostream buf(str);
            buf << err;
            llvm::report_fatal_error(str);
        }
    }

    struct hash_stream64 : public llvm::raw_ostream {
        uint64_t inner;
        uint64_t _current_pos;

        hash_stream64() : inner(14695981039346656037ull), _current_pos(0) {
            SetUnbuffered();
        }

        explicit operator uint64_t() const {
            return inner;
        }

        void write_impl(const char *ptr, size_t size) override {
            for (int i = 0; i < size; i++) {
                inner = (uint64_t(ptr[i]) ^ inner) * 1099511628211ull;
            }
            _current_pos += size;
        }

        [[nodiscard]] uint64_t current_pos() const override {
            return _current_pos;
        }
    };

    template<typename FuzzerInsertingType>
    clang::NamedDecl *getDecl(const clang::Type *underlying, bool &isDefinition) {
        auto asDecl = llvm::dyn_cast<FuzzerInsertingType>(underlying)->getDecl();
        if (asDecl == nullptr) {
            llvm::errs() << llvm::formatv("cant not get decl of underlying type {0}",
                                          underlying->getCanonicalTypeInternal().getAsString());
            return nullptr;
        }
        auto defDecl = asDecl->getDefinition();
        isDefinition = defDecl != nullptr;
        return (defDecl ? defDecl : asDecl);
    }

    template<typename FuzzerInsertingType>
    clang::NamedDecl *getDecl(const clang::Type *underlying) {
        bool dummy;
        return getDecl<FuzzerInsertingType>(underlying, dummy);
    }

    clang::NamedDecl *getDecl(const clang::Type *underlying) {
        if (auto typedef_node = llvm::dyn_cast<clang::TypedefType>(underlying)) {
            return typedef_node->getDecl();
        } else if (llvm::isa<clang::EnumType>(underlying)) {
            return getDecl<clang::EnumType>(underlying);
        } else if (llvm::isa<clang::RecordType>(underlying)) {
            return getDecl<clang::RecordType>(underlying);
        } else if (auto fpt = llvm::dyn_cast<clang::FunctionProtoType>(underlying)) {
            return fpt->getExceptionSpecDecl();
        } else if (auto ft = llvm::dyn_cast<clang::FunctionType>(underlying)) {
            return nullptr;
        } else if (auto et = llvm::dyn_cast<clang::ElaboratedType>(underlying)) {
            return et->getOwnedTagDecl();
        } else if (auto pt = llvm::dyn_cast<clang::PointerType>(underlying)) {
            return nullptr;
        } else if (llvm::isa<clang::TypeOfExprType>(underlying)) {
            return nullptr;
        } else if (llvm::isa<clang::BuiltinType>(underlying)) {
            return nullptr;
        } else {
            clang::QualType(underlying, 0).dump();
            llvm::outs() << "type is " <<
                         clang::QualType(underlying, 0).getAsString() << "\n";
            return nullptr;
        }
    }

    uint64_t createTypeMangleDigest(clang::MangleContext &mangle_ctx, const clang::Type *t) {
        hash_stream64 h;
        auto d = getDecl(t);
        if (d) {
            h << d->getName();
        }
        h << clang::QualType(t, 0).getAsString();
        return uint64_t(h);
    }

    uint64_t createVariableMangleDigest(
            clang::MangleContext &mangle_ctx, const clang::Type *t, llvm::StringRef varName) {
        hash_stream64 h;
        auto d = getDecl(t);
        if (d) {
            h << d->getName();
        }
        h << clang::QualType(t, 0).getAsString() << ":" << varName;
        return uint64_t(h);
    }

    uint64_t createFunctionMangleDigest(
            clang::MangleContext &mangle_ctx, const clang::FunctionType *t, llvm::StringRef funcName) {
        hash_stream64 h;
        auto xt = clang::QualType(t, 0);
        mangle_ctx.mangleCXXRTTI(xt, h);
        h << funcName;
        return uint64_t(h);
    }


//    uint64_t digest(llvm::StringRef addition) {
//        hash_stream64 h;
//        h << addition;
//        return uint64_t(h);
//    }

}
