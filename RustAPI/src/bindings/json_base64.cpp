#include "helpers.hpp"

extern "C" {
    char* geode_json_parse(const char* json_str) {
        auto res = matjson::Value::parse(json_str);
        if (res.isErr()) return nullptr;
        return alloc_string(res.unwrap().dump());
    }
    char* geode_json_dump(const char* json_val, int indent) {
        auto val = matjson::parse(json_val).unwrapOrDefault();
        return alloc_string(val.dump(indent));
    }

    char* geode_base64_encode(const char* data, int variant) {
        auto res = utils::base64::encode(data, static_cast<utils::base64::Base64Variant>(variant));
        return alloc_string(res);
    }
    char* geode_base64_decode(const char* data, int variant) {
        auto res = utils::base64::decode(data, static_cast<utils::base64::Base64Variant>(variant));
        if (res.isErr()) return nullptr;
        auto bytes = res.unwrap();
        char* out = (char*)malloc(bytes.size());
        memcpy(out, bytes.data(), bytes.size());
        return out;
    }
    char* geode_base64_decode_string(const char* data, int variant) {
        auto res = utils::base64::decodeString(data, static_cast<utils::base64::Base64Variant>(variant));
        if (res.isErr()) return nullptr;
        return alloc_string(res.unwrap());
    }
}
