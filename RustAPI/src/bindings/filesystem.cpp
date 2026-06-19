#include "helpers.hpp"

extern "C" {
    char* geode_fs_read(const char* path) {
        auto res = utils::file::readString(std::filesystem::path(path));
        if (res.isErr()) return nullptr;
        return alloc_string(res.unwrap());
    }
    bool geode_fs_write(const char* path, const char* data) {
        return utils::file::writeString(std::filesystem::path(path), data).isOk();
    }
    bool geode_fs_exists(const char* path) {
        std::error_code ec;
        return std::filesystem::exists(std::filesystem::path(path), ec);
    }
    char* geode_fs_list(const char* path) {
        auto res = utils::file::readDirectory(std::filesystem::path(path));
        if (res.isErr()) return nullptr;
        matjson::Value arr = matjson::Value::array();
        for (auto const& entry : res.unwrap()) {
            arr.push(utils::string::pathToString(entry.filename()));
        }
        return alloc_string(arr.dump());
    }
    bool geode_fs_mkdir(const char* path) {
        return utils::file::createDirectoryAll(std::filesystem::path(path)).isOk();
    }
    bool geode_fs_remove(const char* path) {
        std::error_code ec;
        return std::filesystem::remove_all(std::filesystem::path(path), ec) > 0;
    }
}
