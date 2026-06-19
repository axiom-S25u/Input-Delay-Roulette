#include "helpers.hpp"

typedef void (*RustKeyboardCallback)(int key, int action, int mods, double time);
static RustKeyboardCallback g_keyboardCb = nullptr;

struct KeyboardHook : cocos2d::CCNode, CCKeyboardDelegate {
    bool init() override {
        return cocos2d::CCNode::init();
    }
    CREATE_FUNC(KeyboardHook);

    void keyDown(enumKeyCodes key, double time) override {
        if (g_keyboardCb) g_keyboardCb((int)key, 1, 0, time);
    }
    void keyUp(enumKeyCodes key, double time) override {
        if (g_keyboardCb) g_keyboardCb((int)key, 0, 0, time);
    }
};

static KeyboardHook* g_kbHook = nullptr;

extern "C" {
    void geode_keyboard_listen(RustKeyboardCallback cb) {
        g_keyboardCb = cb;
    }
}

// called from main.cpp on mod load
void rustapi_init_keyboard() {
    g_kbHook = KeyboardHook::create();
    g_kbHook->retain();
    CCDirector::get()->getKeyboardDispatcher()->addDelegate(g_kbHook);
}

void rustapi_shutdown_keyboard() {
    if (g_kbHook) {
        CCDirector::get()->getKeyboardDispatcher()->removeDelegate(g_kbHook);
        g_kbHook->release();
        g_kbHook = nullptr;
    }
}
