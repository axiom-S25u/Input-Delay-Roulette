#include "helpers.hpp"

extern "C" {
    char* geode_version_parse(const char* version_str) {
        auto res = VersionInfo::parse(version_str);
        if (res.isErr()) return nullptr;
        auto v = res.unwrap();
        matjson::Value obj = matjson::Value::object();
        obj.set("major", v.getMajor());
        obj.set("minor", v.getMinor());
        obj.set("patch", v.getPatch());
        return alloc_string(obj.dump());
    }
    int geode_version_compare(const char* a, const char* b) {
        auto ra = VersionInfo::parse(a);
        auto rb = VersionInfo::parse(b);
        if (ra.isErr() || rb.isErr()) return 0;
        auto va = ra.unwrap();
        auto vb = rb.unwrap();
        return va < vb ? -1 : (va == vb ? 0 : 1);
    }
    bool geode_version_matches(const char* constraint, const char* version) {
        auto rc = ComparableVersionInfo::parse(constraint);
        auto rv = VersionInfo::parse(version);
        if (rc.isErr() || rv.isErr()) return false;
        return rc.unwrap().compare(rv.unwrap());
    }

    void geode_color_define(const char* id, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        ccColor4B c; c.r = r; c.g = g; c.b = b; c.a = a;
        ColorProvider::get()->define(id, c);
    }
    void geode_color_override(const char* id, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        ccColor4B c; c.r = r; c.g = g; c.b = b; c.a = a;
        ColorProvider::get()->override(id, c);
    }
    void geode_color_reset(const char* id) {
        ColorProvider::get()->reset(id);
    }
    void geode_color_color(const char* id, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
        auto c = ColorProvider::get()->color(id);
        *r = c.r; *g = c.g; *b = c.b; *a = c.a;
    }
    void geode_color_color3b(const char* id, uint8_t* r, uint8_t* g, uint8_t* b) {
        auto c = ColorProvider::get()->color3b(id);
        *r = c.r; *g = c.g; *b = c.b;
    }
}
