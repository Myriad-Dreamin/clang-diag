//
// Created by kamiyoru on 2020/10/22.
//

#ifndef KERNEL_FUZZER_COMMON_ERROR_CATEGORY_H
#define KERNEL_FUZZER_COMMON_ERROR_CATEGORY_H

#include <system_error>

namespace kfuzz {

    struct ErrorCategory;

    extern struct ErrorCategory fps_error_cat;

    struct ErrorCategory : public std::error_category {

        /// inherit from llvm clang tool return code
        /// \returns 0 on success; 1 if any error occurred; 2 if there is no error but
        /// some files are skipped due to missing compile commands.
#define _FPS_EC() \
        _FPS_PEC(OK)\
        _FPS_PEC(Error)\
        _FPS_PEC(SomeFileSkipped)

        enum ErrorCode {
#define _FPS_PEC(Tok) Tok,
            _FPS_EC()
#undef _FPS_PEC
        };

        [[nodiscard]] const char *name() const noexcept override {
            return "fuzzer-fps-err";
        }

        [[nodiscard]] std::string message(int errVal) const override {
            switch (errVal) {
#define _FPS_PEC(Tok) case Tok:\
                return #Tok;
                _FPS_EC()
#undef _FPS_PEC
                default:
                    return "<unknown>";
            }
        }
    };

}


#endif //KERNEL_FUZZER_COMMON_ERROR_CATEGORY_H
