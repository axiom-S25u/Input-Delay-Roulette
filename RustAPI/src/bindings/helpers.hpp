#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/coro.hpp>
#include <Geode/utils/base64.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <matjson.hpp>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstring>

using namespace geode::prelude;

inline char* alloc_string(const std::string& s) {
    char* res = (char*)malloc(s.size() + 1);
    memcpy(res, s.c_str(), s.size() + 1);
    return res;
}
