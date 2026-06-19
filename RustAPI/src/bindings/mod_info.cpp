#include "helpers.hpp"

extern "C" {
    char* geode_mod_get_id() {
        return alloc_string(Mod::get()->getID());
    }
    char* geode_mod_get_name() {
        return alloc_string(Mod::get()->getName());
    }
    char* geode_mod_get_version() {
        return alloc_string(Mod::get()->getVersion().toVString());
    }
    char* geode_mod_get_resources_dir() {
        return alloc_string(utils::string::pathToString(Mod::get()->getResourcesDir()));
    }
    char* geode_mod_get_save_dir() {
        return alloc_string(utils::string::pathToString(Mod::get()->getSaveDir()));
    }
    char* geode_mod_get_config_dir() {
        return alloc_string(utils::string::pathToString(Mod::get()->getConfigDir()));
    }
    char* geode_mod_get_persistent_dir() {
        return alloc_string(utils::string::pathToString(Mod::get()->getPersistentDir()));
    }

    char* geode_mod_get_saved_value(const char* key) {
        auto res = Mod::get()->getSavedValue<matjson::Value>(key);
        return alloc_string(res.dump());
    }
    void geode_mod_set_saved_value(const char* key, const char* json_val) {
        auto val = matjson::parse(json_val).unwrapOrDefault();
        Mod::get()->setSavedValue(key, val);
    }
}
