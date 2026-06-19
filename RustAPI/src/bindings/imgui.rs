use crate::ffi;
use std::ffi::CString;

pub struct ImGuiContext {}

impl ImGuiContext {
    pub fn new() -> Self {
        Self {}
    }

    pub fn init_host() {
        unsafe { ffi::imgui_init_host() }
    }

    pub fn shutdown_host() {
        unsafe { ffi::imgui_shutdown_host() }
    }

    pub fn set_visible(visible: bool) {
        unsafe { ffi::imgui_set_visible(visible) }
    }

    pub fn toggle() {
        unsafe { ffi::imgui_toggle() }
    }

    pub fn is_visible() -> bool {
        unsafe { ffi::imgui_is_visible() }
    }

    pub fn register_draw_callback(cb: extern "C" fn()) {
        unsafe { ffi::imgui_register_draw_callback(cb) }
    }

    pub fn begin_window(&self, name: &str) -> bool {
        let c_name = CString::new(name).unwrap();
        unsafe { ffi::imgui_begin_window(c_name.as_ptr()) }
    }

    pub fn end_window(&self) {
        unsafe { ffi::imgui_end_window() }
    }

    pub fn begin_child(&self, str_id: &str) -> bool {
        let c_id = CString::new(str_id).unwrap();
        unsafe { ffi::imgui_begin_child(c_id.as_ptr()) }
    }

    pub fn end_child(&self) {
        unsafe { ffi::imgui_end_child() }
    }

    pub fn text(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::imgui_text(c_text.as_ptr()) }
    }

    pub fn text_wrapped(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::imgui_text_wrapped(c_text.as_ptr()) }
    }

    pub fn bullet_text(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::imgui_bullet_text(c_text.as_ptr()) }
    }

    pub fn button(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_button(c_label.as_ptr()) }
    }

    pub fn checkbox(&self, label: &str, value: &mut bool) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_checkbox(c_label.as_ptr(), value) }
    }

    pub fn radio_button(&self, label: &str, active: bool) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_radio_button(c_label.as_ptr(), active) }
    }

    pub fn slider_float(&self, label: &str, value: &mut f32, min: f32, max: f32) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_slider_float(c_label.as_ptr(), value, min, max) }
    }

    pub fn slider_int(&self, label: &str, value: &mut i32, min: i32, max: i32) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_slider_int(c_label.as_ptr(), value, min, max) }
    }

    pub fn drag_float(&self, label: &str, value: &mut f32, speed: f32, min: f32, max: f32) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_drag_float(c_label.as_ptr(), value, speed, min, max) }
    }

    pub fn drag_int(&self, label: &str, value: &mut i32, speed: f32, min: i32, max: i32) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_drag_int(c_label.as_ptr(), value, speed, min, max) }
    }

    pub fn same_line(&self) {
        unsafe { ffi::imgui_same_line() }
    }

    pub fn separator(&self) {
        unsafe { ffi::imgui_separator() }
    }

    pub fn separator_text(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::imgui_separator_text(c_text.as_ptr()) }
    }

    pub fn spacing(&self) {
        unsafe { ffi::imgui_spacing() }
    }

    pub fn new_line(&self) {
        unsafe { ffi::imgui_new_line() }
    }

    pub fn indent(&self, w: f32) {
        unsafe { ffi::imgui_indent(w) }
    }

    pub fn unindent(&self, w: f32) {
        unsafe { ffi::imgui_unindent(w) }
    }

    pub fn dummy(&self, x: f32, y: f32) {
        unsafe { ffi::imgui_dummy(x, y) }
    }

    pub fn columns(&self, count: i32, id: &str, border: bool) {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_columns(count, c_id.as_ptr(), border) }
    }

    pub fn next_column(&self) {
        unsafe { ffi::imgui_next_column() }
    }

    pub fn collapsing_header(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_collapsing_header(c_label.as_ptr()) }
    }

    pub fn tree_node(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_tree_node(c_label.as_ptr()) }
    }

    pub fn tree_pop(&self) {
        unsafe { ffi::imgui_tree_pop() }
    }

    pub fn begin_group(&self) {
        unsafe { ffi::imgui_begin_group() }
    }

    pub fn end_group(&self) {
        unsafe { ffi::imgui_end_group() }
    }

    pub fn push_id_str(&self, id: &str) {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_push_id_str(c_id.as_ptr()) }
    }

    pub fn push_id_int(&self, id: i32) {
        unsafe { ffi::imgui_push_id_int(id) }
    }

    pub fn pop_id(&self) {
        unsafe { ffi::imgui_pop_id() }
    }

    pub fn begin_table(&self, id: &str, columns: i32) -> bool {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_begin_table(c_id.as_ptr(), columns) }
    }

    pub fn end_table(&self) {
        unsafe { ffi::imgui_end_table() }
    }

    pub fn table_next_row(&self) {
        unsafe { ffi::imgui_table_next_row() }
    }

    pub fn table_next_column(&self) -> bool {
        unsafe { ffi::imgui_table_next_column() }
    }

    pub fn table_setup_column(&self, label: &str) {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_table_setup_column(c_label.as_ptr()) }
    }

    pub fn table_headers_row(&self) {
        unsafe { ffi::imgui_table_headers_row() }
    }

    pub fn table_set_column_index(&self, column: i32) -> bool {
        unsafe { ffi::imgui_table_set_column_index(column) }
    }

    pub fn open_popup(&self, id: &str) {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_open_popup(c_id.as_ptr()) }
    }

    pub fn begin_popup(&self, id: &str) -> bool {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_begin_popup(c_id.as_ptr()) }
    }

    pub fn end_popup(&self) {
        unsafe { ffi::imgui_end_popup() }
    }

    pub fn begin_popup_modal(&self, name: &str) -> bool {
        let c_name = CString::new(name).unwrap();
        unsafe { ffi::imgui_begin_popup_modal(c_name.as_ptr()) }
    }

    pub fn close_current_popup(&self) {
        unsafe { ffi::imgui_close_current_popup() }
    }

    pub fn begin_tab_bar(&self, id: &str) -> bool {
        let c_id = CString::new(id).unwrap();
        unsafe { ffi::imgui_begin_tab_bar(c_id.as_ptr()) }
    }

    pub fn end_tab_bar(&self) {
        unsafe { ffi::imgui_end_tab_bar() }
    }

    pub fn begin_tab_item(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_begin_tab_item(c_label.as_ptr()) }
    }

    pub fn end_tab_item(&self) {
        unsafe { ffi::imgui_end_tab_item() }
    }

    pub fn set_tooltip(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::imgui_set_tooltip(c_text.as_ptr()) }
    }

    pub fn begin_menu_bar(&self) -> bool {
        unsafe { ffi::imgui_begin_menu_bar() }
    }

    pub fn end_menu_bar(&self) {
        unsafe { ffi::imgui_end_menu_bar() }
    }

    pub fn begin_menu(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_begin_menu(c_label.as_ptr()) }
    }

    pub fn end_menu(&self) {
        unsafe { ffi::imgui_end_menu() }
    }

    pub fn menu_item(&self, label: &str) -> bool {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::imgui_menu_item(c_label.as_ptr()) }
    }

    pub fn style_colors_dark(&self) {
        unsafe { ffi::imgui_style_colors_dark() }
    }

    pub fn style_colors_light(&self) {
        unsafe { ffi::imgui_style_colors_light() }
    }

    pub fn style_colors_classic(&self) {
        unsafe { ffi::imgui_style_colors_classic() }
    }

    pub fn push_style_var_float(&self, idx: i32, val: f32) {
        unsafe { ffi::imgui_push_style_var_float(idx, val) }
    }

    pub fn push_style_var_vec2(&self, idx: i32, x: f32, y: f32) {
        unsafe { ffi::imgui_push_style_var_vec2(idx, x, y) }
    }

    pub fn pop_style_var(&self, count: i32) {
        unsafe { ffi::imgui_pop_style_var(count) }
    }

    pub fn push_style_color(&self, idx: i32, r: f32, g: f32, b: f32, a: f32) {
        unsafe { ffi::imgui_push_style_color(idx, r, g, b, a) }
    }

    pub fn pop_style_color(&self, count: i32) {
        unsafe { ffi::imgui_pop_style_color(count) }
    }
}
