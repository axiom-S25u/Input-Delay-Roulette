#include "helpers.hpp"

extern "C" {
    void geode_create_quick_popup(const char* title, const char* content, const char* btn1, const char* btn2, void (*cb)(bool)) {
        createQuickPopup(title, content, btn1, btn2, [cb](auto*, bool b2) {
            if (cb) cb(b2);
        });
    }
}
