//
// Created by kamiyoru on 2020/11/2.
//

#include <llvm/Support/CommandLine.h>
#include "clang/Frontend/SerializedDiagnosticReader.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <kfuzz/Common.h>
#include <kfuzz/Diag.h>

namespace kfuzz::clang_diag {

    enum class OutputType {
        To_stdout,
        To_file,
    } res;

    OutputType parseOutputType(llvm::StringRef output_file) {
        if (output_file.equals("stdout")) {
            return OutputType::To_stdout;
        } else {
            return OutputType::To_file;
        }
    }

    enum class FormatStyle {
        JSON,
        YAML,
        Plugin,
    };
}

namespace kfuzz {


    template<>
    void marshalJSON<clang_diag::ExtendedClangDiagnosticRecord>(
            nlohmann::json &j, const clang_diag::ExtendedClangDiagnosticRecord &obj) {
        marshalJSON(j, *static_cast<const ClangDiagnosticRecord *>(&obj));
        json stack = json::array();
        for (auto &loc: obj.stack) {
            json j2;
            marshalJSON(j2, loc);
            stack.emplace_back(j2);
        }
        j["stack"] = stack;
        json expansion_note = json::array();
        for (auto &en: obj.expansion_note) {
            json j2;
            marshalJSON(j2, en);
            expansion_note.emplace_back(j2);
        }
        j["expansion_note"] = expansion_note;
    }
}

namespace kfuzz::clang_diag {

    template<typename T>
    std::error_code std_raw_job_func_json_impl(
            typename std_raw_job<T>::type &job) {
        using nlohmann::json;
        json j = json::array();
        for (auto record = job.file.nextRecord(); record != nullptr; record = job.file.nextRecord()) {
            json s_record = json::object();
            s_record["type"] = record->type;
            switch (record->type) {
                case ClangDiagnosticRecordBase::SourceRangeRecord :
                    marshalJSON(s_record, *reinterpret_cast<ClangSourceRangeRecord *>(record));
                    break;
                case ClangDiagnosticRecordBase::CategoryRecord:
                    marshalJSON(s_record, *reinterpret_cast<ClangCategoryRecord *>(record));
                    break;
                case ClangDiagnosticRecordBase::DiagFlagRecord:
                    marshalJSON(s_record, *reinterpret_cast<ClangDiagFlagRecord *>(record));
                    break;
                case ClangDiagnosticRecordBase::DiagnosticRecord:
                    marshalJSON(s_record, *reinterpret_cast<ClangDiagnosticRecord *>(record));
                    break;
                case ClangDiagnosticRecordBase::FilenameRecord:
                    marshalJSON(s_record, *reinterpret_cast<ClangFilenameRecord *>(record));
                    break;
                case ClangDiagnosticRecordBase::Unknown:
                default:
                    llvm::errs() << "unknown type of record (" << record->type << ")";
                    break;
            }
            j.emplace_back(s_record);
        }

        *job.ostream << j;
        return {};
    }

    std::error_code std_job_func_json_impl(
            typename std_job::type &job) {
        json j;
        j["files"] = job.result.file_map;
        j["flags"] = job.result.flag_map;
        j["categories"] = job.result.category_map;
        json diagnostics = json::array();
        for (auto &diag: job.result.records) {
            json j2;
            marshalJSON(j2, diag);
            diagnostics.emplace_back(j2);
        }
        j["diagnostics"] = diagnostics;

        *job.ostream << j;
        return {};
    }
}

int main(int argc, const char **argv) {

    using namespace kfuzz::clang_diag;

    // add options
    static llvm::cl::OptionCategory diag_opts_cat("clang-diag options");

    static llvm::cl::opt<FormatStyle> output_format(
            "format", llvm::cl::cat(diag_opts_cat),
            llvm::cl::init(FormatStyle::JSON),
            llvm::cl::values(
                    clEnumValN(FormatStyle::JSON, "json", "in json format"),
                    clEnumValN(FormatStyle::YAML, "yaml", "in yaml format"),
                    clEnumValN(FormatStyle::Plugin, "plugin",
                               "use format from that in dynamic plugin")),
            llvm::cl::desc("format diag to format"));

    static llvm::cl::opt<std::string> raw_output_opt(
            "output", llvm::cl::cat(diag_opts_cat),
            llvm::cl::init("stdout"),
            llvm::cl::desc("the output path, default 'stdout'"));

    static llvm::cl::alias raw_output_opt_short(
            "o", llvm::cl::aliasopt(raw_output_opt), llvm::cl::cat(diag_opts_cat),
            llvm::cl::desc("alias for --output"));

    static llvm::cl::opt<bool> raw_style(
            "raw", llvm::cl::cat(diag_opts_cat),
            llvm::cl::desc("simply convert serialized file to target format instead of "
                           "changing the restructured info of diagnostics file at the same time"));

    static llvm::cl::alias raw_style_short(
            "r", llvm::cl::aliasopt(raw_style), llvm::cl::cat(diag_opts_cat),
            llvm::cl::desc("alias for --raw"));

    static llvm::cl::opt<std::string> input_file(
            llvm::cl::Positional, llvm::cl::Required, llvm::cl::cat(diag_opts_cat),
            llvm::cl::desc("<input file>"));

    static llvm::cl::opt<bool> verbose(
            "verbose", llvm::cl::cat(diag_opts_cat),
            llvm::cl::desc("see verbose info"));

    auto verbose_outs = [&]() -> llvm::raw_ostream & {
        if (verbose) {
            return llvm::outs();
        } else {
            return llvm::nulls();
        }
    };

    // add extra help
    static llvm::cl::extrahelp diag_help("\nclang diagnostics tool\n");

    llvm::cl::ParseCommandLineOptions(argc, argv);

    /// parse diagnostics file
    ClangDiagnosticsFile file;
    SerializedDiagnosticsFileConverter converter(file);
    converter.readDiagnostics(input_file.getValue());

    verbose_outs() << "read " << file.size() << " records from diagnostics file\n";

    ClangDiagnosticsResult result;
    if (!raw_style) {
        verbose_outs() << "converting diagnostics file to restructured info...\n";
        result.parseAppend(IClangDiagnosticsFile<ClangDiagnosticsFile>(file));
    }

    /// write formatted struct to output
    auto output = raw_output_opt.getValue();

    if (output_format.getValue() == FormatStyle::JSON) {
        verbose_outs() << "" << input_file.getValue() << " > [json-formatter] > " << output << "\n";
        auto output_type = parseOutputType(output);
        std::ostream *o;

        if (output_type == OutputType::To_stdout) {
            o = &std::cout;
        } else {
            o = new std::ofstream(output);
        }
        *o << std::setw(2);

        if (raw_style) {
            verbose_outs() << "dumping raw info...\n";
            auto job = std_raw_job<ClangDiagnosticsFile>::create(
                    IClangDiagnosticsFile<ClangDiagnosticsFile>(file), o);
            std_raw_job_func_json_impl<ClangDiagnosticsFile>(job);
        } else {
            verbose_outs() << "dumping restructured info...\n";
            auto job = std_job::create(result, o);
            std_job_func_json_impl(job);
//            result
        }

        if (output_type != OutputType::To_stdout) {
            (reinterpret_cast<std::ofstream *>(o))->close();
            delete o;
        } else {
            *o << "\n";
        }
    }


    return 0;
}
