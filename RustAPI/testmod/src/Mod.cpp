#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <RustAPI.hpp>

using namespace geode::prelude;

extern "C" {
    void testmod_init();
    void testmod_shutdown();
    void testmod_show_popup();
}

$on_mod(Loaded) {
    log::info("Loading Rust testmod...");
    testmod_init();
}

struct $modify(MenuLayerHook, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        testmod_show_popup();
        
        return true;
    }
};