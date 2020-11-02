//
// Created by kamiyoru on 2020/11/2.
//

#include <kfuzz/Diag/Serialize.h>

namespace kfuzz {
    template<>
    void marshalJSON<location_t>(json &j, const location_t &obj) {
        j["file_id"] = obj.FileID;
        j["line"] = obj.Line;
        j["col"] = obj.Col;
        j["offset"] = obj.Offset;
    }

    template<>
    void marshalJSON<ClangFilenameRecord>(nlohmann::json &j, const ClangFilenameRecord &obj) {
        j["id"] = obj.id;
        j["name"] = obj.name;
        j["timestamp"] = obj.timestamp;
        j["size"] = obj.size;
    }

    template<>
    void marshalJSON<ClangDiagnosticRecord>(nlohmann::json &j, const ClangDiagnosticRecord &obj) {
        j["location"] = nlohmann::json::object();
        marshalJSON(j["location"], obj.location);
        j["severity"] = obj.severity;
        j["category"] = obj.category;
        j["flag"] = obj.flag;
        j["msg"] = obj.msg;
    }

    template<>
    void marshalJSON<ClangSourceRangeRecord>(nlohmann::json &j, const ClangSourceRangeRecord &obj) {
        j["begin_loc"] = nlohmann::json::object();
        marshalJSON(j["begin_loc"], obj.begin_loc);
        j["end_loc"] = nlohmann::json::object();
        marshalJSON(j["end_loc"], obj.end_loc);
    }

    template<>
    void marshalJSON<ClangCategoryRecord>(nlohmann::json &j, const ClangCategoryRecord &obj) {
        j["id"] = obj.id;
        j["name"] = obj.name;
    }

    template<>
    void marshalJSON<ClangDiagFlagRecord>(nlohmann::json &j, const ClangDiagFlagRecord &obj) {
        j["id"] = obj.id;
        j["name"] = obj.name;
    }

    template<>
    void unmarshalJSON<location_t>(const json &j, location_t &obj) {
        obj.FileID = j["file_id"];
        obj.Line = j["line"];
        obj.Col = j["col"];
        obj.Offset = j["offset"];
    }

    template<>
    void unmarshalJSON<ClangFilenameRecord>(const json &j, ClangFilenameRecord &obj) {
        obj.id = j["id"];
        obj.name = j["name"];
        obj.timestamp = j["timestamp"];
        obj.size = j["size"];
    }

    template<>
    void unmarshalJSON<ClangDiagnosticRecord>(const json &j, ClangDiagnosticRecord &obj) {
        unmarshalJSON(j["location"], obj.location);
        obj.severity = j["severity"];
        obj.category = j["category"];
        obj.flag = j["flag"];
        obj.msg = j["msg"];
    }

    template<>
    void unmarshalJSON<ClangSourceRangeRecord>(const json &j, ClangSourceRangeRecord &obj) {
        unmarshalJSON(j["begin_loc"], obj.begin_loc);
        unmarshalJSON(j["end_loc"], obj.end_loc);
    }

    template<>
    void unmarshalJSON<ClangCategoryRecord>(const json &j, ClangCategoryRecord &obj) {
        obj.id = j["id"];
        obj.name = j["name"];
    }

    template<>
    void unmarshalJSON<ClangDiagFlagRecord>(const json &j, ClangDiagFlagRecord &obj) {
        obj.id = j["id"];
        obj.name = j["name"];
    }
}
