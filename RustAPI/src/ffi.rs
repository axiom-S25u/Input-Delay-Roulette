use std::ffi::{CString};
use std::os::raw::c_char;
use std::os::raw::c_void;

#[repr(C)]
pub struct GeodeResult {
    is_ok: bool,
    error: *mut c_char,
}

impl GeodeResult {
    pub fn ok() -> Self {
        Self {
            is_ok: true,
            error: std::ptr::null_mut(),
        }
    }

    pub fn err(msg: String) -> Self {
        let err = CString::new(msg).unwrap();
        Self {
            is_ok: false,
            error: err.into_raw(),
        }
    }
}

#[repr(C)]
pub struct GeodeMod {
    _private: [u8; 0],
}

#[repr(C)]
pub struct GeodeNode {
    _private: [u8; 0],
}

#[repr(C)]
pub struct GeodeLayer {
    _private: [u8; 0],
}

#[repr(C)]
pub struct GeodeSprite {
    _private: [u8; 0],
}

pub type RustKeyboardCallback = extern "C" fn(key: i32, action: i32, mods: i32, time: f64);
pub type RustWebCallback = extern "C" fn(status: i32, body: *const c_char, error: *const c_char);

extern "C" {
    pub fn geode_mod_get() -> *mut GeodeMod;
    pub fn geode_mod_get_id(mod_ptr: *mut GeodeMod) -> *const c_char;
    pub fn geode_mod_get_name(mod_ptr: *mut GeodeMod) -> *const c_char;
    pub fn geode_mod_get_version(mod_ptr: *mut GeodeMod) -> *mut c_char;
    pub fn geode_mod_get_save_dir(mod_ptr: *mut GeodeMod) -> *mut c_char;
    pub fn geode_mod_get_resources_dir(mod_ptr: *mut GeodeMod) -> *mut c_char;
    pub fn geode_mod_get_config_dir(mod_ptr: *mut GeodeMod) -> *mut c_char;

    pub fn geode_mod_get_saved_value(mod_ptr: *mut GeodeMod, key: *const c_char) -> *mut c_char;
    pub fn geode_mod_set_saved_value(mod_ptr: *mut GeodeMod, key: *const c_char, json_val: *const c_char);

    pub fn geode_log_info(msg: *const c_char);
    pub fn geode_log_warn(msg: *const c_char);
    pub fn geode_log_error(msg: *const c_char);
    pub fn geode_keybind_from_string(s: *const i8) -> *mut i8;
    pub fn geode_keybind_to_string(key: i32, modifiers: i32) -> *mut i8;

    // ImGui
    pub fn imgui_init_host();
    pub fn imgui_shutdown_host();
    pub fn imgui_set_visible(visible: bool);
    pub fn imgui_toggle();
    pub fn imgui_is_visible() -> bool;
    pub fn imgui_register_draw_callback(cb: extern "C" fn());

    pub fn imgui_begin_window(name: *const i8) -> bool;
    pub fn imgui_end_window();
    pub fn imgui_begin_child(str_id: *const i8) -> bool;
    pub fn imgui_end_child();

    pub fn imgui_text(text: *const i8);
    pub fn imgui_text_wrapped(text: *const i8);
    pub fn imgui_bullet_text(text: *const i8);
    pub fn imgui_button(label: *const i8) -> bool;
    pub fn imgui_checkbox(label: *const i8, v: *mut bool) -> bool;
    pub fn imgui_radio_button(label: *const i8, active: bool) -> bool;
    pub fn imgui_slider_float(label: *const i8, v: *mut f32, v_min: f32, v_max: f32) -> bool;
    pub fn imgui_slider_int(label: *const i8, v: *mut i32, v_min: i32, v_max: i32) -> bool;
    pub fn imgui_drag_float(label: *const i8, v: *mut f32, v_speed: f32, v_min: f32, v_max: f32) -> bool;
    pub fn imgui_drag_int(label: *const i8, v: *mut i32, v_speed: f32, v_min: i32, v_max: i32) -> bool;

    pub fn imgui_same_line();
    pub fn imgui_separator();
    pub fn imgui_separator_text(text: *const i8);
    pub fn imgui_spacing();
    pub fn imgui_new_line();
    pub fn imgui_indent(w: f32);
    pub fn imgui_unindent(w: f32);
    pub fn imgui_dummy(x: f32, y: f32);
    pub fn imgui_columns(count: i32, id: *const i8, border: bool);
    pub fn imgui_next_column();
    pub fn imgui_collapsing_header(label: *const i8) -> bool;
    pub fn imgui_tree_node(label: *const i8) -> bool;
    pub fn imgui_tree_pop();
    pub fn imgui_begin_group();
    pub fn imgui_end_group();

    pub fn imgui_push_id_str(id: *const i8);
    pub fn imgui_push_id_int(id: i32);
    pub fn imgui_pop_id();

    pub fn imgui_begin_table(id: *const i8, columns: i32) -> bool;
    pub fn imgui_end_table();
    pub fn imgui_table_next_row();
    pub fn imgui_table_next_column() -> bool;
    pub fn imgui_table_setup_column(label: *const i8);
    pub fn imgui_table_headers_row();
    pub fn imgui_table_set_column_index(column: i32) -> bool;

    pub fn imgui_open_popup(id: *const i8);
    pub fn imgui_begin_popup(id: *const i8) -> bool;
    pub fn imgui_end_popup();
    pub fn imgui_begin_popup_modal(name: *const i8) -> bool;
    pub fn imgui_close_current_popup();
    pub fn imgui_begin_tab_bar(id: *const i8) -> bool;
    pub fn imgui_end_tab_bar();
    pub fn imgui_begin_tab_item(label: *const i8) -> bool;
    pub fn imgui_end_tab_item();
    pub fn imgui_set_tooltip(text: *const i8);

    pub fn imgui_begin_menu_bar() -> bool;
    pub fn imgui_end_menu_bar();
    pub fn imgui_begin_menu(label: *const i8) -> bool;
    pub fn imgui_end_menu();
    pub fn imgui_menu_item(label: *const i8) -> bool;

    pub fn imgui_style_colors_dark();
    pub fn imgui_style_colors_light();
    pub fn imgui_style_colors_classic();
    pub fn imgui_push_style_var_float(idx: i32, val: f32);
    pub fn imgui_push_style_var_vec2(idx: i32, x: f32, y: f32);
    pub fn imgui_pop_style_var(count: i32);
    pub fn imgui_push_style_color(idx: i32, r: f32, g: f32, b: f32, a: f32);
    pub fn imgui_pop_style_color(count: i32);

    pub fn geode_node_get_children(node: *mut GeodeNode) -> *mut c_void;
    pub fn geode_node_add_child(node: *mut GeodeNode, child: *mut GeodeNode);
    pub fn geode_node_remove_from_parent(node: *mut GeodeNode);
    pub fn geode_layer_create() -> *mut GeodeLayer;
    pub fn geode_layer_add_child(layer: *mut GeodeLayer, child: *mut GeodeNode);
    pub fn geode_sprite_create(texture_name: *const c_char) -> *mut GeodeSprite;
    pub fn cocos_node_get_child_by_id(node: *mut GeodeNode, id: *const c_char) -> *mut GeodeNode;

    // Task
    pub fn geode_task_run_main(cb: extern "C" fn(*mut c_void), data: *mut c_void);

    pub fn geode_fs_read(path: *const c_char) -> *mut c_char;
    pub fn geode_fs_write(path: *const c_char, data: *const c_char) -> bool;
    pub fn geode_fs_exists(path: *const c_char) -> bool;
    pub fn geode_fs_list(path: *const c_char) -> *mut c_char;
    pub fn geode_fs_mkdir(path: *const c_char) -> bool;
    pub fn geode_fs_remove(path: *const c_char) -> bool;

    pub fn cocos_node_set_pos(n: *mut GeodeNode, x: f32, y: f32);
    pub fn cocos_node_get_pos(n: *mut GeodeNode, x: *mut f32, y: *mut f32);
    pub fn cocos_node_set_scale(n: *mut GeodeNode, s: f32);
    pub fn cocos_node_get_scale(n: *mut GeodeNode) -> f32;
    pub fn cocos_node_set_rot(n: *mut GeodeNode, r: f32);
    pub fn cocos_node_get_rot(n: *mut GeodeNode) -> f32;
    pub fn cocos_node_set_visible(n: *mut GeodeNode, v: bool);
    pub fn cocos_node_is_visible(n: *mut GeodeNode) -> bool;
    pub fn cocos_node_set_opacity(n: *mut GeodeNode, o: u8);
    pub fn cocos_node_get_opacity(n: *mut GeodeNode) -> u8;
    pub fn cocos_node_set_color(n: *mut GeodeNode, r: u8, g: u8, b: u8);
    pub fn cocos_node_get_color(n: *mut GeodeNode, r: *mut u8, g: *mut u8, b: *mut u8);

    pub fn geode_keyboard_listen(cb: Option<RustKeyboardCallback>);
    pub fn geode_web_fetch(method: *const c_char, url: *const c_char, body: *const c_char, cb: RustWebCallback);

    pub fn geode_create_quick_popup(
        title: *const c_char,
        content: *const c_char,
        btn1: *const c_char,
        btn2: *const c_char,
        cb: extern "C" fn(bool)
    );

    pub fn imgui_style_color(idx: i32, r: f32, g: f32, b: f32, a: f32);

    pub fn geode_render3d_set_camera(x: f32, y: f32, z: f32, rx: f32, ry: f32, rz: f32);

    pub fn rustapi_free_string(s: *mut c_char);
}

pub fn log_info(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { geode_log_info(c_msg.as_ptr()) };
}

pub fn log_warn(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { geode_log_warn(c_msg.as_ptr()) };
}

pub fn log_error(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { geode_log_error(c_msg.as_ptr()) };
}