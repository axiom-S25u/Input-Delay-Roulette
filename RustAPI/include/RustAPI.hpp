#pragma once

#include <Geode/Geode.hpp>
#include <cocos2d.h>

#if defined(GEODE_IS_WINDOWS)
    #ifdef RUSTAPI_EXPORTING
        #define RUSTAPI_DLL __declspec(dllexport)
    #else
        #define RUSTAPI_DLL __declspec(dllimport)
    #endif
#else
    #ifdef RUSTAPI_EXPORTING
        #define RUSTAPI_DLL __attribute__((visibility("default")))
    #else
        #define RUSTAPI_DLL
    #endif
#endif

extern "C" {
    // Logging
    RUSTAPI_DLL void geode_log_info(const char* msg);
    RUSTAPI_DLL void geode_log_warn(const char* msg);
    RUSTAPI_DLL void geode_log_error(const char* msg);

    // Popup
    RUSTAPI_DLL void geode_create_quick_popup(
        const char* title,
        const char* content,
        const char* btn1,
        const char* btn2,
        void (*callback)(bool)
    );

    // Keyboard
    typedef void (*RustKeyboardCallback)(int key, int action, int mods, double time);
    RUSTAPI_DLL void geode_keyboard_listen(RustKeyboardCallback cb);

    // Mod info
    RUSTAPI_DLL char* geode_mod_get_id();
    RUSTAPI_DLL char* geode_mod_get_name();
    RUSTAPI_DLL char* geode_mod_get_version();
    RUSTAPI_DLL char* geode_mod_get_resources_dir();
    RUSTAPI_DLL char* geode_mod_get_save_dir();
    RUSTAPI_DLL char* geode_mod_get_config_dir();
    RUSTAPI_DLL char* geode_mod_get_persistent_dir();

    // Saved values
    RUSTAPI_DLL char* geode_mod_get_saved_value(const char* key);
    RUSTAPI_DLL void geode_mod_set_saved_value(const char* key, const char* json_val);

    // Filesystem
    RUSTAPI_DLL char* geode_fs_read(const char* path);
    RUSTAPI_DLL bool geode_fs_write(const char* path, const char* data);
    RUSTAPI_DLL bool geode_fs_exists(const char* path);
    RUSTAPI_DLL char* geode_fs_list(const char* path);
    RUSTAPI_DLL bool geode_fs_mkdir(const char* path);
    RUSTAPI_DLL bool geode_fs_remove(const char* path);

    // JSON
    RUSTAPI_DLL char* geode_json_parse(const char* json_str);
    RUSTAPI_DLL char* geode_json_dump(const char* json_val, int indent);

    // Base64
    RUSTAPI_DLL char* geode_base64_encode(const char* data, int variant);
    RUSTAPI_DLL char* geode_base64_decode(const char* data, int variant);
    RUSTAPI_DLL char* geode_base64_decode_string(const char* data, int variant);

    // Version info
    RUSTAPI_DLL char* geode_version_parse(const char* version_str);
    RUSTAPI_DLL int geode_version_compare(const char* a, const char* b);
    RUSTAPI_DLL bool geode_version_matches(const char* constraint, const char* version);

    // Color provider
    RUSTAPI_DLL void geode_color_define(const char* id, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    RUSTAPI_DLL void geode_color_override(const char* id, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    RUSTAPI_DLL void geode_color_reset(const char* id);
    RUSTAPI_DLL void geode_color_color(const char* id, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);
    RUSTAPI_DLL void geode_color_color3b(const char* id, uint8_t* r, uint8_t* g, uint8_t* b);

    // Keybind
    RUSTAPI_DLL char* geode_keybind_from_string(const char* str);
    RUSTAPI_DLL char* geode_keybind_to_string(int key, int modifiers);

    // ImGui
    RUSTAPI_DLL void imgui_init_host();
    RUSTAPI_DLL void imgui_shutdown_host();
    RUSTAPI_DLL void imgui_set_visible(bool visible);
    RUSTAPI_DLL void imgui_toggle();
    RUSTAPI_DLL bool imgui_is_visible();
    
    typedef void (*RustImGuiDrawCallback)();
    RUSTAPI_DLL void imgui_register_draw_callback(RustImGuiDrawCallback cb);

    RUSTAPI_DLL bool imgui_begin_window(const char* name);
    RUSTAPI_DLL void imgui_end_window();
    RUSTAPI_DLL bool imgui_begin_child(const char* str_id);
    RUSTAPI_DLL void imgui_end_child();

    RUSTAPI_DLL void imgui_text(const char* text);
    RUSTAPI_DLL void imgui_text_wrapped(const char* text);
    RUSTAPI_DLL void imgui_bullet_text(const char* text);
    RUSTAPI_DLL bool imgui_button(const char* label);
    RUSTAPI_DLL bool imgui_checkbox(const char* label, bool* v);
    RUSTAPI_DLL bool imgui_radio_button(const char* label, bool active);
    RUSTAPI_DLL bool imgui_slider_float(const char* label, float* v, float v_min, float v_max);
    RUSTAPI_DLL bool imgui_slider_int(const char* label, int* v, int v_min, int v_max);
    RUSTAPI_DLL bool imgui_drag_float(const char* label, float* v, float v_speed, float v_min, float v_max);
    RUSTAPI_DLL bool imgui_drag_int(const char* label, int* v, float v_speed, int v_min, int v_max);

    RUSTAPI_DLL void imgui_same_line();
    RUSTAPI_DLL void imgui_separator();
    RUSTAPI_DLL void imgui_separator_text(const char* text);
    RUSTAPI_DLL void imgui_spacing();
    RUSTAPI_DLL void imgui_new_line();
    RUSTAPI_DLL void imgui_indent(float w);
    RUSTAPI_DLL void imgui_unindent(float w);
    RUSTAPI_DLL void imgui_dummy(float x, float y);
    RUSTAPI_DLL void imgui_columns(int count, const char* id, bool border);
    RUSTAPI_DLL void imgui_next_column();
    RUSTAPI_DLL bool imgui_collapsing_header(const char* label);
    RUSTAPI_DLL bool imgui_tree_node(const char* label);
    RUSTAPI_DLL void imgui_tree_pop();
    RUSTAPI_DLL void imgui_begin_group();
    RUSTAPI_DLL void imgui_end_group();

    RUSTAPI_DLL void imgui_push_id_str(const char* id);
    RUSTAPI_DLL void imgui_push_id_int(int id);
    RUSTAPI_DLL void imgui_pop_id();

    RUSTAPI_DLL bool imgui_begin_table(const char* id, int columns);
    RUSTAPI_DLL void imgui_end_table();
    RUSTAPI_DLL void imgui_table_next_row();
    RUSTAPI_DLL bool imgui_table_next_column();
    RUSTAPI_DLL void imgui_table_setup_column(const char* label);
    RUSTAPI_DLL void imgui_table_headers_row();
    RUSTAPI_DLL bool imgui_table_set_column_index(int column);

    RUSTAPI_DLL void imgui_open_popup(const char* id);
    RUSTAPI_DLL bool imgui_begin_popup(const char* id);
    RUSTAPI_DLL void imgui_end_popup();
    RUSTAPI_DLL bool imgui_begin_popup_modal(const char* name);
    RUSTAPI_DLL void imgui_close_current_popup();
    RUSTAPI_DLL bool imgui_begin_tab_bar(const char* id);
    RUSTAPI_DLL void imgui_end_tab_bar();
    RUSTAPI_DLL bool imgui_begin_tab_item(const char* label);
    RUSTAPI_DLL void imgui_end_tab_item();
    RUSTAPI_DLL void imgui_set_tooltip(const char* text);

    RUSTAPI_DLL bool imgui_begin_menu_bar();
    RUSTAPI_DLL void imgui_end_menu_bar();
    RUSTAPI_DLL bool imgui_begin_menu(const char* label);
    RUSTAPI_DLL void imgui_end_menu();
    RUSTAPI_DLL bool imgui_menu_item(const char* label);

    RUSTAPI_DLL void imgui_style_colors_dark();
    RUSTAPI_DLL void imgui_style_colors_light();
    RUSTAPI_DLL void imgui_style_colors_classic();
    RUSTAPI_DLL void imgui_push_style_var_float(int idx, float val);
    RUSTAPI_DLL void imgui_push_style_var_vec2(int idx, float x, float y);
    RUSTAPI_DLL void imgui_pop_style_var(int count);
    RUSTAPI_DLL void imgui_push_style_color(int idx, float r, float g, float b, float a);
    RUSTAPI_DLL void imgui_pop_style_color(int count);

    // Web
    typedef void (*RustWebCallback)(int status, const char* body, const char* error);
    RUSTAPI_DLL void geode_web_fetch(const char* method, const char* url, const char* body, RustWebCallback cb);
    RUSTAPI_DLL void geode_web_open_link(const char* url);

    // Cocos nodes
    RUSTAPI_DLL void cocos_node_set_pos(void* node, float x, float y);
    RUSTAPI_DLL void cocos_node_get_pos(void* node, float* x, float* y);
    RUSTAPI_DLL void cocos_node_set_scale(void* node, float s);
    RUSTAPI_DLL float cocos_node_get_scale(void* node);
    RUSTAPI_DLL void cocos_node_set_rot(void* node, float r);
    RUSTAPI_DLL float cocos_node_get_rot(void* node);
    RUSTAPI_DLL void cocos_node_set_visible(void* node, bool v);
    RUSTAPI_DLL bool cocos_node_is_visible(void* node);
    RUSTAPI_DLL void cocos_node_set_opacity(void* node, uint8_t o);
    RUSTAPI_DLL uint8_t cocos_node_get_opacity(void* node);
    RUSTAPI_DLL void cocos_node_set_color(void* node, uint8_t r, uint8_t g, uint8_t b);
    RUSTAPI_DLL void cocos_node_get_color(void* node, uint8_t* r, uint8_t* g, uint8_t* b);
    RUSTAPI_DLL void geode_node_add_child(void* node, void* child);
    RUSTAPI_DLL void geode_node_remove_from_parent(void* node);

    // String free
    RUSTAPI_DLL void geode_free_string(char* s);
}
