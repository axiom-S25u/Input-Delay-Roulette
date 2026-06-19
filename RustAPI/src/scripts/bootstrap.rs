#[path = "../../testmod.rs"]
mod testmod;
use crate::bindings::geode;

pub fn init() {
    geode::log_info("RustAPI bootstrap script loaded");
    testmod::init();
}
