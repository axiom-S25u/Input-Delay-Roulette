#include <Geode/Geode.hpp>
#include <RustAPI.hpp>
#include <filesystem>

using namespace geode::prelude;

extern "C" {
    char* rustapi_init();
    void rustapi_shutdown();
    char* rustapi_run_file(const char* path);
    void rustapi_free_string(char* s);
}

void rustapi_init_keyboard();
void rustapi_shutdown_keyboard();

$on_mod(Loaded) {
    rustapi_init_keyboard();

    char* err = rustapi_init();
    if (err) {
        log::error("RustAPI init failed: {}", err);
        rustapi_free_string(err);
        return;
    }

    log::info("RustAPI loaded successfully");

    std::filesystem::path bootstrapPath = Mod::get()->getResourcesDir() / "scripts" / "bootstrap.rs";
    if (std::filesystem::exists(bootstrapPath)) {
        char* runErr = rustapi_run_file(bootstrapPath.string().c_str());
        if (runErr) {
            log::error("RustAPI bootstrap failed: {}", runErr);
            rustapi_free_string(runErr);
        }
    }
}

$on_game(Exiting) {
    rustapi_shutdown_keyboard();
    rustapi_shutdown();
}
