//
// Created by kamiyoru on 2020/10/17.
//

#ifndef KERNEL_FUZZER_COMMON_LOGGER_H
#define KERNEL_FUZZER_COMMON_LOGGER_H

#include "llvm/Support/Chrono.h"
#include "llvm/Support/FormatAdapters.h"
#include "llvm/Support/FormatVariadic.h"

namespace kfuzz {
    class Logger {
    public:
        virtual ~Logger() = default;

        static const char *level_str[];
        enum Level {
            Critical, Error, Warning, Info, Debug, Trace, All
        };

        template<typename... Ts>
        void critical(const char *Fmt, Ts &&... args) {
            log(Critical, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

        template<typename... Ts>
        void error(const char *Fmt, Ts &&... args) {
            log(Error, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

        template<typename... Ts>
        void warning(const char *Fmt, Ts &&... args) {
            log(Warning, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

        template<typename... Ts>
        void info(const char *Fmt, Ts &&... args) {
            log(Info, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

        template<typename... Ts>
        void debug(const char *Fmt, Ts &&... args) {
            log(Debug, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

        template<typename... Ts>
        void trace(const char *Fmt, Ts &&... args) {
            log(Trace, llvm::formatv(Fmt, std::forward<Ts>(args)...));
        }

    protected:
        virtual void log(Level Level, const llvm::formatv_object_base &Message) = 0;
    };

    class StreamLogger : public Logger {
        llvm::raw_ostream &inner_stream;
        Logger::Level min_level;
    public:
        StreamLogger(llvm::raw_ostream &inner_stream, Logger::Level min_level);

        ~StreamLogger() override = default;

    protected:
        void log(Logger::Level Level, const llvm::formatv_object_base &Message) override;
    };

    llvm::Expected<Logger *> createFileLogger(llvm::StringRef fileName, llvm::raw_fd_ostream **stream_handle);
}

#endif //KERNEL_FUZZER_COMMON_LOGGER_H
