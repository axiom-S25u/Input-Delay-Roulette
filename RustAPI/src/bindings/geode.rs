use crate::ffi;
use std::ffi::{CStr, CString};
use std::path::Path;
use serde_json::Value;

pub struct Mod {
    _ptr: *mut ffi::GeodeMod,
}

impl Mod {
    pub fn get() -> Self {
        Self {
            _ptr: unsafe { ffi::geode_mod_get() },
        }
    }

    pub fn get_id(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_id(self._ptr);
            if ptr.is_null() { return String::new(); }
            CStr::from_ptr(ptr).to_string_lossy().into_owned()
        }
    }

    pub fn get_name(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_name(self._ptr);
            if ptr.is_null() { return String::new(); }
            CStr::from_ptr(ptr).to_string_lossy().into_owned()
        }
    }

    pub fn get_version(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_version(self._ptr);
            if ptr.is_null() { return String::new(); }
            let s = CStr::from_ptr(ptr).to_string_lossy().into_owned();
            ffi::rustapi_free_string(ptr);
            s
        }
    }

    pub fn get_save_dir(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_save_dir(self._ptr);
            if ptr.is_null() { return String::new(); }
            let s = CStr::from_ptr(ptr).to_string_lossy().into_owned();
            ffi::rustapi_free_string(ptr);
            s
        }
    }

    pub fn get_resources_dir(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_resources_dir(self._ptr);
            if ptr.is_null() { return String::new(); }
            let s = CStr::from_ptr(ptr).to_string_lossy().into_owned();
            ffi::rustapi_free_string(ptr);
            s
        }
    }

    pub fn get_config_dir(&self) -> String {
        unsafe {
            let ptr = ffi::geode_mod_get_config_dir(self._ptr);
            if ptr.is_null() { return String::new(); }
            let s = CStr::from_ptr(ptr).to_string_lossy().into_owned();
            ffi::rustapi_free_string(ptr);
            s
        }
    }

    pub fn get_saved_value(&self, key: &str) -> Result<Value, String> {
        let c_key = CString::new(key).map_err(|e| e.to_string())?;
        unsafe {
            let ptr = ffi::geode_mod_get_saved_value(self._ptr, c_key.as_ptr());
            if ptr.is_null() { return Ok(Value::Null); }
            let s = CStr::from_ptr(ptr).to_string_lossy();
            let val = serde_json::from_str(&s).map_err(|e| e.to_string())?;
            ffi::rustapi_free_string(ptr);
            Ok(val)
        }
    }

    pub fn set_saved_value(&self, key: &str, value: &Value) -> Result<(), String> {
        let c_key = CString::new(key).map_err(|e| e.to_string())?;
        let json_val = serde_json::to_string(value).map_err(|e| e.to_string())?;
        let c_val = CString::new(json_val).map_err(|e| e.to_string())?;
        unsafe {
            ffi::geode_mod_set_saved_value(self._ptr, c_key.as_ptr(), c_val.as_ptr());
        }
        Ok(())
    }
}

pub struct Node {
    pub(crate) _ptr: *mut ffi::GeodeNode,
}

impl Node {
    pub fn set_position(&self, x: f32, y: f32) {
        unsafe { ffi::cocos_node_set_pos(self._ptr, x, y) }
    }

    pub fn get_position(&self) -> (f32, f32) {
        let mut x = 0.0;
        let mut y = 0.0;
        unsafe { ffi::cocos_node_get_pos(self._ptr, &mut x, &mut y) }
        (x, y)
    }

    pub fn set_scale(&self, s: f32) {
        unsafe { ffi::cocos_node_set_scale(self._ptr, s) }
    }

    pub fn get_scale(&self) -> f32 {
        unsafe { ffi::cocos_node_get_scale(self._ptr) }
    }

    pub fn set_rotation(&self, r: f32) {
        unsafe { ffi::cocos_node_set_rot(self._ptr, r) }
    }

    pub fn get_rotation(&self) -> f32 {
        unsafe { ffi::cocos_node_get_rot(self._ptr) }
    }

    pub fn set_visible(&self, v: bool) {
        unsafe { ffi::cocos_node_set_visible(self._ptr, v) }
    }

    pub fn is_visible(&self) -> bool {
        unsafe { ffi::cocos_node_is_visible(self._ptr) }
    }

    pub fn set_opacity(&self, o: u8) {
        unsafe { ffi::cocos_node_set_opacity(self._ptr, o) }
    }

    pub fn get_opacity(&self) -> u8 {
        unsafe { ffi::cocos_node_get_opacity(self._ptr) }
    }

    pub fn set_color(&self, r: u8, g: u8, b: u8) {
        unsafe { ffi::cocos_node_set_color(self._ptr, r, g, b) }
    }

    pub fn get_color(&self) -> (u8, u8, u8) {
        let mut r = 0;
        let mut g = 0;
        let mut b = 0;
        unsafe { ffi::cocos_node_get_color(self._ptr, &mut r, &mut g, &mut b) }
        (r, g, b)
    }

    pub fn add_child(&self, child: &Node) {
        unsafe { ffi::geode_node_add_child(self._ptr, child._ptr) }
    }

    pub fn remove_from_parent(&self) {
        unsafe { ffi::geode_node_remove_from_parent(self._ptr) }
    }
}

pub fn log_info(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { ffi::geode_log_info(c_msg.as_ptr()) };
}

pub fn log_warn(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { ffi::geode_log_warn(c_msg.as_ptr()) };
}

pub fn log_error(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { ffi::geode_log_error(c_msg.as_ptr()) };
}

static mut POPUP_CALLBACK: Option<Box<dyn Fn(bool) + Send>> = None;

extern "C" fn popup_cb(b2: bool) {
    unsafe {
        if let Some(ref cb) = POPUP_CALLBACK {
            cb(b2);
        }
    }
}

pub fn create_quick_popup<F>(title: &str, content: &str, btn1: &str, btn2: &str, callback: F)
where
    F: Fn(bool) + Send + 'static,
{
    let c_title = CString::new(title).unwrap();
    let c_content = CString::new(content).unwrap();
    let c_btn1 = CString::new(btn1).unwrap();
    let c_btn2 = CString::new(btn2).unwrap();

    unsafe {
        POPUP_CALLBACK = Some(Box::new(callback));
        ffi::geode_create_quick_popup(
            c_title.as_ptr(),
            c_content.as_ptr(),
            c_btn1.as_ptr(),
            c_btn2.as_ptr(),
            popup_cb
        );
    }
}

pub mod fs {
    use super::*;

    pub fn read(path: &Path) -> Result<String, String> {
        let c_path = CString::new(path.to_string_lossy().as_ref()).map_err(|e| e.to_string())?;
        unsafe {
            let ptr = ffi::geode_fs_read(c_path.as_ptr());
            if ptr.is_null() { return Err("failed to read file".to_string()); }
            let s = CStr::from_ptr(ptr).to_string_lossy().into_owned();
            ffi::rustapi_free_string(ptr);
            Ok(s)
        }
    }

    pub fn write(path: &Path, data: &str) -> Result<(), String> {
        let c_path = CString::new(path.to_string_lossy().as_ref()).map_err(|e| e.to_string())?;
        let c_data = CString::new(data).map_err(|e| e.to_string())?;
        if unsafe { ffi::geode_fs_write(c_path.as_ptr(), c_data.as_ptr()) } {
            Ok(())
        } else {
            Err("failed to write file".to_string())
        }
    }

    pub fn exists(path: &Path) -> bool {
        let c_path = CString::new(path.to_string_lossy().as_ref()).unwrap();
        unsafe { ffi::geode_fs_exists(c_path.as_ptr()) }
    }

    pub fn list(path: &Path) -> Result<Vec<String>, String> {
        let c_path = CString::new(path.to_string_lossy().as_ref()).map_err(|e| e.to_string())?;
        unsafe {
            let ptr = ffi::geode_fs_list(c_path.as_ptr());
            if ptr.is_null() { return Err("failed to list directory".to_string()); }
            let s = CStr::from_ptr(ptr).to_string_lossy();
            let val: Value = serde_json::from_str(&s).map_err(|e| e.to_string())?;
            ffi::rustapi_free_string(ptr);
            if let Some(arr) = val.as_array() {
                Ok(arr.iter().filter_map(|v| v.as_str().map(|s| s.to_string())).collect())
            } else {
                Err("invalid directory list response".to_string())
            }
        }
    }

    pub fn mkdir(path: &Path) -> Result<(), String> {
        let c_path = CString::new(path.to_string_lossy().as_ref()).map_err(|e| e.to_string())?;
        if unsafe { ffi::geode_fs_mkdir(c_path.as_ptr()) } {
            Ok(())
        } else {
            Err("failed to create directory".to_string())
        }
    }

    pub fn remove(path: &Path) -> Result<(), String> {
        let c_path = CString::new(path.to_string_lossy().as_ref()).map_err(|e| e.to_string())?;
        if unsafe { ffi::geode_fs_remove(c_path.as_ptr()) } {
            Ok(())
        } else {
            Err("failed to remove file/directory".to_string())
        }
    }
}

pub mod web {
    use super::*;

    static mut CALLBACK: Option<Box<dyn Fn(i32, String, Option<String>) + Send>> = None;

    extern "C" fn web_cb(status: i32, body: *const std::os::raw::c_char, error: *const std::os::raw::c_char) {
        unsafe {
            if let Some(ref cb) = CALLBACK {
                let body_str = if body.is_null() { String::new() } else { CStr::from_ptr(body).to_string_lossy().into_owned() };
                let error_str = if error.is_null() { None } else { Some(CStr::from_ptr(error).to_string_lossy().into_owned()) };
                cb(status, body_str, error_str);
            }
        }
    }

    pub fn fetch<F>(method: &str, url: &str, body: Option<&str>, callback: F)
    where
        F: Fn(i32, String, Option<String>) + Send + 'static,
    {
        let c_method = CString::new(method).unwrap();
        let c_url = CString::new(url).unwrap();
        let c_body = CString::new(body.unwrap_or("")).unwrap();
        
        unsafe {
            CALLBACK = Some(Box::new(callback));
            ffi::geode_web_fetch(c_method.as_ptr(), c_url.as_ptr(), c_body.as_ptr(), web_cb);
        }
    }
}

pub mod keyboard {
    use super::*;

    static mut CALLBACK: Option<Box<dyn Fn(i32, i32, i32, f64) + Send>> = None;

    extern "C" fn keyboard_cb(key: i32, action: i32, mods: i32, time: f64) {
        unsafe {
            if let Some(ref cb) = CALLBACK {
                cb(key, action, mods, time);
            }
        }
    }

    pub fn listen<F>(callback: F)
    where
        F: Fn(i32, i32, i32, f64) + Send + 'static,
    {
        unsafe {
            CALLBACK = Some(Box::new(callback));
            ffi::geode_keyboard_listen(Some(keyboard_cb));
        }
    }

    pub fn stop_listening() {
        unsafe {
            CALLBACK = None;
            ffi::geode_keyboard_listen(None);
        }
    }
}

pub mod json {
    use serde::Serialize;
    use serde_json::Value;

    pub fn parse(text: &str) -> Result<Value, String> {
        serde_json::from_str(text).map_err(|e| e.to_string())
    }

    pub fn dump(value: &Value, indent: Option<usize>) -> Result<String, String> {
        if let Some(indent) = indent {
            let mut s = Vec::new();
            let indent_bytes = vec![b' '; indent];
            let formatter = serde_json::ser::PrettyFormatter::with_indent(&indent_bytes);
            let mut ser = serde_json::Serializer::with_formatter(&mut s, formatter);
            value.serialize(&mut ser).map_err(|e| e.to_string())?;
            Ok(String::from_utf8(s).unwrap())
        } else {
            serde_json::to_string(value).map_err(|e| e.to_string())
        }
    }
}
    
    pub fn to_string<T: Serialize>(value: &T) -> Result<String, String> {
        serde_json::to_string(value).map_err(|e| e.to_string())
    }

pub mod base64 {
    use base64::Engine;

    pub fn encode(data: &[u8]) -> String {
        base64::engine::general_purpose::STANDARD.encode(data)
    }

    pub fn decode(data: &str) -> Result<Vec<u8>, String> {
        base64::engine::general_purpose::STANDARD.decode(data).map_err(|e| e.to_string())
    }

    pub fn decode_string(data: &str) -> Result<String, String> {
        let bytes = base64::engine::general_purpose::STANDARD.decode(data).map_err(|e| e.to_string())?;
        String::from_utf8(bytes).map_err(|e| e.to_string())
    }
}

use serde::Serialize;
