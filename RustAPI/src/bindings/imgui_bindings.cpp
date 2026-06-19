#include "helpers.hpp"
#include <imgui.h>
#include <imgui-cocos.hpp>

extern "C" {
    // Core
    void imgui_init_host() {
        ImGuiCocos::get().setup([] {}).draw([] {
            // we will call rust draw callbacks here if we had any
        });
    }

    void imgui_shutdown_host() {
        if (ImGuiCocos::get().isInitialized()) {
            ImGuiCocos::get().destroy();
        }
    }

    void imgui_set_visible(bool visible) {
        ImGuiCocos::get().setVisible(visible);
    }

    void imgui_toggle() {
        ImGuiCocos::get().toggle();
    }

    bool imgui_is_visible() {
        return ImGuiCocos::get().isVisible();
    }

    // A simple callback for the draw loop
    typedef void (*RustImGuiDrawCallback)();
    static RustImGuiDrawCallback g_drawCb = nullptr;

    void imgui_register_draw_callback(RustImGuiDrawCallback cb) {
        g_drawCb = cb;
        ImGuiCocos::get().setup([] {}).draw([] {
            if (g_drawCb) {
                g_drawCb();
            }
        });
    }

    // Windows
    bool imgui_begin_window(const char* name) { return ImGui::Begin(name); }
    void imgui_end_window() { ImGui::End(); }
    bool imgui_begin_child(const char* str_id) { return ImGui::BeginChild(str_id); }
    void imgui_end_child() { ImGui::EndChild(); }

    // Widgets
    void imgui_text(const char* text) { ImGui::TextUnformatted(text); }
    void imgui_text_wrapped(const char* text) { ImGui::TextWrapped("%s", text); }
    void imgui_bullet_text(const char* text) { ImGui::BulletText("%s", text); }
    bool imgui_button(const char* label) { return ImGui::Button(label); }
    bool imgui_checkbox(const char* label, bool* v) { return ImGui::Checkbox(label, v); }
    bool imgui_radio_button(const char* label, bool active) { return ImGui::RadioButton(label, active); }
    
    bool imgui_slider_float(const char* label, float* v, float v_min, float v_max) { 
        return ImGui::SliderFloat(label, v, v_min, v_max); 
    }
    bool imgui_slider_int(const char* label, int* v, int v_min, int v_max) { 
        return ImGui::SliderInt(label, v, v_min, v_max); 
    }
    bool imgui_drag_float(const char* label, float* v, float v_speed, float v_min, float v_max) {
        return ImGui::DragFloat(label, v, v_speed, v_min, v_max);
    }
    bool imgui_drag_int(const char* label, int* v, float v_speed, int v_min, int v_max) {
        return ImGui::DragInt(label, v, v_speed, v_min, v_max);
    }

    // Layout
    void imgui_same_line() { ImGui::SameLine(); }
    void imgui_separator() { ImGui::Separator(); }
    void imgui_separator_text(const char* text) { ImGui::SeparatorText(text); }
    void imgui_spacing() { ImGui::Spacing(); }
    void imgui_new_line() { ImGui::NewLine(); }
    void imgui_indent(float w) { ImGui::Indent(w); }
    void imgui_unindent(float w) { ImGui::Unindent(w); }
    void imgui_dummy(float x, float y) { ImGui::Dummy({x, y}); }
    
    void imgui_columns(int count, const char* id, bool border) { ImGui::Columns(count, id, border); }
    void imgui_next_column() { ImGui::NextColumn(); }

    bool imgui_collapsing_header(const char* label) { return ImGui::CollapsingHeader(label); }
    bool imgui_tree_node(const char* label) { return ImGui::TreeNode(label); }
    void imgui_tree_pop() { ImGui::TreePop(); }
    void imgui_begin_group() { ImGui::BeginGroup(); }
    void imgui_end_group() { ImGui::EndGroup(); }

    // IDs
    void imgui_push_id_str(const char* id) { ImGui::PushID(id); }
    void imgui_push_id_int(int id) { ImGui::PushID(id); }
    void imgui_pop_id() { ImGui::PopID(); }

    // Tables
    bool imgui_begin_table(const char* id, int columns) { return ImGui::BeginTable(id, columns); }
    void imgui_end_table() { ImGui::EndTable(); }
    void imgui_table_next_row() { ImGui::TableNextRow(); }
    bool imgui_table_next_column() { return ImGui::TableNextColumn(); }
    void imgui_table_setup_column(const char* label) { ImGui::TableSetupColumn(label); }
    void imgui_table_headers_row() { ImGui::TableHeadersRow(); }
    bool imgui_table_set_column_index(int column) { return ImGui::TableSetColumnIndex(column); }

    // Popups
    void imgui_open_popup(const char* id) { ImGui::OpenPopup(id); }
    bool imgui_begin_popup(const char* id) { return ImGui::BeginPopup(id); }
    void imgui_end_popup() { ImGui::EndPopup(); }
    bool imgui_begin_popup_modal(const char* name) { return ImGui::BeginPopupModal(name); }
    void imgui_close_current_popup() { ImGui::CloseCurrentPopup(); }
    bool imgui_begin_tab_bar(const char* id) { return ImGui::BeginTabBar(id); }
    void imgui_end_tab_bar() { ImGui::EndTabBar(); }
    bool imgui_begin_tab_item(const char* label) { return ImGui::BeginTabItem(label); }
    void imgui_end_tab_item() { ImGui::EndTabItem(); }
    void imgui_set_tooltip(const char* text) { ImGui::SetTooltip("%s", text); }

    // Menus
    bool imgui_begin_menu_bar() { return ImGui::BeginMenuBar(); }
    void imgui_end_menu_bar() { ImGui::EndMenuBar(); }
    bool imgui_begin_menu(const char* label) { return ImGui::BeginMenu(label); }
    void imgui_end_menu() { ImGui::EndMenu(); }
    bool imgui_menu_item(const char* label) { return ImGui::MenuItem(label); }

    // Style & Theme
    void imgui_style_colors_dark() { ImGui::StyleColorsDark(); }
    void imgui_style_colors_light() { ImGui::StyleColorsLight(); }
    void imgui_style_colors_classic() { ImGui::StyleColorsClassic(); }
    void imgui_push_style_var_float(int idx, float val) { ImGui::PushStyleVar(idx, val); }
    void imgui_push_style_var_vec2(int idx, float x, float y) { ImGui::PushStyleVar(idx, {x, y}); }
    void imgui_pop_style_var(int count) { ImGui::PopStyleVar(count); }
    void imgui_push_style_color(int idx, float r, float g, float b, float a) { ImGui::PushStyleColor(idx, {r, g, b, a}); }
    void imgui_pop_style_color(int count) { ImGui::PopStyleColor(count); }
}
