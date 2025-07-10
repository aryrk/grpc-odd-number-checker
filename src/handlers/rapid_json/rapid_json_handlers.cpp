#include "rapid_json_handlers.h"

template<typename T>
void WriteRapidJSON(rapidjson::Document &document, std::string_view key, T value) {
    if (!document.IsObject()) {
        document.SetObject();
    }
    auto &alloc = document.GetAllocator();
    rapidjson::Value keyVal(key.data(), key.size(), alloc);

    rapidjson::Value val;
    if constexpr (std::is_same_v<T, int>) {
        val.SetInt(value);
    } else if constexpr (std::is_same_v<T, double>) {
        val.SetDouble(value);
    } else if constexpr (std::is_same_v<T, bool>) {
        val.SetBool(value);
    } else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) {
        val.SetString(value.data(), value.size(), alloc);
    } else {
        static_assert(!sizeof(T), "Unsupported type for WriteRapidJSON");
    }

    auto memberItr = document.FindMember(keyVal);
    if (memberItr != document.MemberEnd()) {
        memberItr->value = val;
    } else {
        document.AddMember(keyVal, val, alloc);
    }
}

template void WriteRapidJSON<int>(rapidjson::Document&, std::string_view, int);
template void WriteRapidJSON<double>(rapidjson::Document&, std::string_view, double);
template void WriteRapidJSON<bool>(rapidjson::Document&, std::string_view, bool);
template void WriteRapidJSON<std::string>(rapidjson::Document&, std::string_view, std::string);
template void WriteRapidJSON<std::string_view>(rapidjson::Document&, std::string_view, std::string_view);

std::string ReadRapidJSON(rapidjson::Document &document) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}