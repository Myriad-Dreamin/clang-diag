//
// Created by kamiyoru on 2020/10/17.
//

#include <kfuzz/Common/Logger.h>
#include "llvm/Support/FileSystem.h"

namespace kfuzz {
    const char *Logger::level_str[] = {"Critical", "Error", "Warning", "Info", "Debug", "Trace", "All",};

    llvm::Expected<Logger *> createFileLogger(llvm::StringRef fileName, llvm::raw_fd_ostream **stream_handle) {
        std::error_code ec;
        auto stream = new llvm::raw_fd_ostream(
                fileName, ec, llvm::sys::fs::CD_CreateAlways,
                llvm::sys::fs::FA_Write, llvm::sys::fs::OF_Append);
        if (ec) {
            llvm::outs() << "error when creating " << fileName << ": " << ec.message() << "\n";
            return llvm::createStringError(ec, "create logger file error");
        }
        *stream_handle = stream;
        return new StreamLogger(*stream, Logger::All);
    }

    StreamLogger::StreamLogger(llvm::raw_ostream &inner_stream, Logger::Level min_level) :
            inner_stream(inner_stream), min_level(min_level) {}

    void StreamLogger::log(Logger::Level Level, const llvm::formatv_object_base &Message) {
        if (Level >= min_level) {
            return;
        }

        llvm::sys::TimePoint<std::chrono::nanoseconds> Timestamp = std::chrono::system_clock::now();
        inner_stream << llvm::formatv("{0}[{1:%H:%M:%S.%L}] {2}\n", Logger::level_str[Level], Timestamp, Message);
        inner_stream.flush();
    }
}
