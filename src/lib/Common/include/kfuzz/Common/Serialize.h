//
// Created by kamiyoru on 2020/10/19.
//

#ifndef KERNEL_FUZZER_COMMON_SERIALIZE_H
#define KERNEL_FUZZER_COMMON_SERIALIZE_H

#include <nlohmann/json.hpp>

namespace kfuzz {
    using nlohmann::json;

    /// generic interface for deserializing an object from nlohmann::json object
    template<typename T>
    void unmarshalJSON(const json &j, T &obj);

    /// generic interface for serializing an object to nlohmann::json object
    template<typename T>
    void marshalJSON(json &j, const T &obj);

    /// macro helper to declare the type's json serialization interface
#define macroDeclareNJSONITmpl(Type) \
    template<> void unmarshalJSON<Type>(const json& j, Type &obj);\
    template<> void marshalJSON<Type>(json& j, const Type &obj)
}

#include "Serialize.inc"


#endif //KERNEL_FUZZER_COMMON_SERIALIZE_H
