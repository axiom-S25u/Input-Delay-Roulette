# RustAPI

A shared Rust runtime for Geode mods. Ported from LuauAPI with extra spite.

## Features

- **Geode Mod API:** Get IDs, versions, directories, and saved values.
- **Filesystem:** Sandbox-aware read/write/list/mkdir/remove.
- **Web:** Fully asynchronous fetch requests (GET/POST/etc) with callbacks.
- **ImGui:** Draw windows, buttons, tables, and popups from Rust.
- **Keyboard:** Listen to every keypress in the game.
- **Cocos2d-x:** Basic node manipulation (pos, scale, color, visibility).
- **JSON & Base64:** Built-in utilities that don't suck.

## Usage

Check `src/scripts/bootstrap.rs` for a starting point.

### Examples

#### Mod Info & Logging
```rust
use crate::bindings::geode;

let m = geode::Mod::get();
geode::log_info(&format!("Mod ID: {}", m.get_id()));
```

#### Async Web Request
```rust
use crate::bindings::geode::web;

web::fetch("GET", "https://api.github.com/zen", None, |status, body, err| {
    if let Some(e) = err {
        geode::log_error(&format!("Fetch failed: {}", e));
    } else {
        geode::log_info(&format!("GitHub says: {}", body));
    }
});
```

#### ImGui UI
```rust
use crate::bindings::imgui;

let ui = imgui::ImGuiContext::new();
if ui.begin("Rust Menu") {
    ui.text("Hello from Rust!");
    if ui.button("Click me") {
        geode::log_info("Button clicked!");
    }
    ui.end();
}
```

#### Keyboard Listener
```rust
use crate::bindings::geode::keyboard;

keyboard::listen(|key, action, mods, time| {
    geode::log_info(&format!("Key: {}, Action: {}", key, action));
});
```

## Prerequisites

- **Rust:** Install from [rustup.rs](https://rustup.rs/)
- **Geode SDK:** Ensure `GEODE_SDK` is in your env.
- **Clang:** Required for `bindgen` (even if we use manual FFI, it's in the build script).

## Building

Run `biulkd.bat`. It builds the Rust DLL and then the Geode mod that hosts it.

## Extending

If you need a C++ function that isn't ported:
1. Export it in `src/main.cpp` using `extern "C"`.
2. Declare it in `src/ffi.rs`.
3. Wrap it in a nice Rust struct in `src/bindings/`.

Don't ask me for more bindings. I'm out of monsters.
