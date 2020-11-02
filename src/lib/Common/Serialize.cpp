//
// Created by kamiyoru on 2020/10/24.
//

#include "kfuzz/Common/Serialize.h"

namespace kfuzz {

    template<>
    void unmarshalJSON<ParseResult>(const json &j, ParseResult &obj) {
        for (auto &info: j["serial_type_decls"]) {
            ParsedDeclInfo pi;
            unmarshalJSON(info, pi);
            auto id = pi.id;
            obj.type_decls.emplace(id, std::move(pi));
            obj.serial_type_decls.push_back(&obj.type_decls[id]);
        }
        for (auto &info: j["serial_func_decls"]) {
            ParsedFunctionInfo pi;
            unmarshalJSON(info, pi);
            auto id = pi.id;
            obj.func_decls.emplace(id, std::move(pi));
            obj.serial_func_decls.push_back(&obj.func_decls[id]);
        }
        for (auto &info: j["serial_var_decls"]) {
            ParsedExternalVarInfo pi;
            unmarshalJSON(info, pi);
            auto id = pi.id;
            obj.var_decls.emplace(id, std::move(pi));
            obj.serial_var_decls.push_back(&obj.var_decls[id]);
        }
    }

    template<>
    void marshalJSON<ParseResult>(json &j, const ParseResult &obj) {
        json json_serial_type_decls_array = json::array();
        for (auto &i : obj.serial_type_decls) {
            json si;
            marshalJSON(si, *i);
            json_serial_type_decls_array.emplace_back(si);
        }
        j["serial_type_decls"] = json_serial_type_decls_array;
        json json_serial_func_decls_array = json::array();
        for (auto &i : obj.serial_func_decls) {
            json si;
            marshalJSON(si, *i);
            json_serial_func_decls_array.emplace_back(si);
        }
        j["serial_func_decls"] = json_serial_func_decls_array;
        json json_serial_var_decls_array = json::array();
        for (auto &i : obj.serial_var_decls) {
            json si;
            marshalJSON(si, *i);
            json_serial_var_decls_array.emplace_back(si);
        }
        j["serial_var_decls"] = json_serial_var_decls_array;
    }

    template<>
    void unmarshalJSON<ParsedDeclInfo>(const json &j, ParsedDeclInfo &obj) {
        obj.id = j["id"].get<uint64_t>();
        obj.type = j["type"].get<std::string>();
        obj.decl = j["decl"].get<std::string>();
        obj.name = j["name"].get<std::string>();
        obj.absFilePath = j["absFilePath"].get<std::string>();
        obj.filePath = j["filePath"].get<std::string>();
        obj.dependency = j["dependency"].get<std::vector<uint64_t>>();
        obj.trait = j["trait"].get<uint64_t>();
        obj.file_offset = j["file_offset"].get<uint64_t>();
    }

    template<>
    void marshalJSON<ParsedDeclInfo>(json &j, const ParsedDeclInfo &obj) {
        j["id"] = obj.id;
        j["type"] = obj.type;
        j["decl"] = obj.decl;
        j["name"] = obj.name;
        j["absFilePath"] = obj.absFilePath;
        j["filePath"] = obj.filePath;
        j["dependency"] = obj.dependency;
        j["trait"] = obj.trait;
        j["file_offset"] = obj.file_offset;
    }

    template<>
    void unmarshalJSON<ParsedFunctionInfo>(const json &j, ParsedFunctionInfo &obj) {
        obj.id = j["id"].get<uint64_t>();
        obj.type = j["type"].get<std::string>();
        obj.signature = j["signature"].get<std::string>();
        obj.body = j["body"].get<std::string>();
        obj.name = j["name"].get<std::string>();
        obj.absFilePath = j["absFilePath"].get<std::string>();
        obj.filePath = j["filePath"].get<std::string>();
        obj.dependency = j["dependency"].get<std::vector<uint64_t>>();
        obj.trait = j["trait"].get<uint64_t>();
        obj.file_offset = j["file_offset"].get<uint64_t>();
    }

    template<>
    void marshalJSON<ParsedFunctionInfo>(json &j, const ParsedFunctionInfo &obj) {
        j["id"] = obj.id;
        j["type"] = obj.type;
        j["signature"] = obj.signature;
        j["body"] = obj.body;
        j["name"] = obj.name;
        j["dependency"] = obj.dependency;
        j["absFilePath"] = obj.absFilePath;
        j["filePath"] = obj.filePath;
        j["trait"] = obj.trait;
        j["file_offset"] = obj.file_offset;
    }

    template<>
    void unmarshalJSON<ParsedExternalVarInfo>(const json &j, ParsedExternalVarInfo &obj) {
        obj.id = j["id"].get<uint64_t>();
        obj.type = j["type"].get<std::string>();
        obj.decl = j["decl"].get<std::string>();
        obj.name = j["name"].get<std::string>();
        obj.absFilePath = j["absFilePath"].get<std::string>();
        obj.filePath = j["filePath"].get<std::string>();
        obj.dependency = j["dependency"].get<std::vector<uint64_t>>();
        obj.file_offset = j["file_offset"].get<uint64_t>();
    }

    template<>
    void marshalJSON<ParsedExternalVarInfo>(json &j, const ParsedExternalVarInfo &obj) {
        j["id"] = obj.id;
        j["type"] = obj.type;
        j["decl"] = obj.decl;
        j["name"] = obj.name;
        j["dependency"] = obj.dependency;
        j["absFilePath"] = obj.absFilePath;
        j["filePath"] = obj.filePath;
        j["file_offset"] = obj.file_offset;
    }
}
