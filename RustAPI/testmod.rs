use crate::bindings::geode;
use crate::bindings::geode::keyboard;

pub fn init() {
    geode::log_info("TestMod initialized from Root!");

    keyboard::listen(|key, action, _mods, _time| {
        if key == 0x4B && action == 0 {
            geode::log_info("K key pressed! Showing popup...");
            geode::create_quick_popup(
                "RustAPI Test",
                "You pressed the K key! This is running from the root folder.",
                "Nah",
                "Hell yeah",
                |clicked_btn2| {
                    if clicked_btn2 {
                        geode::log_info("User thinks it is hell yeah cool");
                    } else {
                        geode::log_info("User is a hater");
                    }
                }
            );
        }
    });
}
