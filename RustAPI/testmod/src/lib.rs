// RustAPI TestMod - actual mod logic in Rust
// This demonstrates using RustAPI bindings from Rust code

use std::ffi::CString;

// FFI declarations to match the C++ side
extern "C" {
    fn geode_log_info(msg: *const i8);
    fn geode_log_warn(msg: *const i8);
    fn geode_log_error(msg: *const i8);
    
    fn geode_create_quick_popup(
        title: *const i8,
        content: *const i8,
        btn1: *const i8,
        btn2: *const i8,
        callback: extern "C" fn(bool)
    );

    fn geode_mod_get_id() -> *const i8;
    fn geode_mod_get_name() -> *const i8;
    fn geode_mod_get_version() -> *const i8;
    fn geode_mod_get_save_dir() -> *const i8;

    fn geode_fs_write(path: *const i8, data: *const i8) -> bool;
    fn geode_fs_read(path: *const i8) -> *const i8;

    fn geode_free_string(s: *mut i8);
}

static mut POPUP_CALLBACK: Option<Box<dyn Fn(bool) + Send>> = None;

extern "C" fn popup_callback(clicked_btn2: bool) {
    unsafe {
        if let Some(ref cb) = POPUP_CALLBACK {
            cb(clicked_btn2);
        }
    }
}

fn create_quick_popup<F>(title: &str, content: &str, btn1: &str, btn2: &str, callback: F)
where
    F: Fn(bool) + Send + 'static,
{
    let c_title = CString::new(title).unwrap();
    let c_content = CString::new(content).unwrap();
    let c_btn1 = CString::new(btn1).unwrap();
    let c_btn2 = CString::new(btn2).unwrap();
    
    unsafe {
        POPUP_CALLBACK = Some(Box::new(callback));
        geode_create_quick_popup(
            c_title.as_ptr(),
            c_content.as_ptr(),
            c_btn1.as_ptr(),
            c_btn2.as_ptr(),
            popup_callback
        );
    }
}

fn log_info(msg: &str) {
    let c_msg = CString::new(msg).unwrap();
    unsafe { geode_log_info(c_msg.as_ptr()) };
}

fn cstr_to_string(ptr: *const i8) -> String {
    unsafe {
        if ptr.is_null() {
            return String::from("(null)");
        }
        std::ffi::CStr::from_ptr(ptr)
            .to_string_lossy()
            .into_owned()
    }
}

// Called from MenuLayer::init
#[no_mangle] 
pub extern "C" fn testmod_show_popup() {
    log_info("Showing popup from Rust on MenuLayer init...");
    
    // Test mod info bindings
    unsafe {
        let id = cstr_to_string(geode_mod_get_id());
        let name = cstr_to_string(geode_mod_get_name());
        let version = cstr_to_string(geode_mod_get_version());
        let save_dir = cstr_to_string(geode_mod_get_save_dir());
        
        log_info(&format!("Mod ID: {}", id));
        log_info(&format!("Mod Name: {}", name));
        log_info(&format!("Mod Version: {}", version));
        log_info(&format!("Save Dir: {}", save_dir));
        
        // Test filesystem bindings
        let test_path = CString::new(save_dir + "\\test.txt").unwrap();
        let test_data = CString::new("Hello from Rust!").unwrap();
        
        if geode_fs_write(test_path.as_ptr(), test_data.as_ptr()) {
            log_info("Successfully wrote test file");
            
            let read_data = geode_fs_read(test_path.as_ptr());
            if !read_data.is_null() {
                let content = cstr_to_string(read_data);
                log_info(&format!("Read back: {}", content));
                geode_free_string(read_data as *mut i8);
            }
        }
    }
    
    create_quick_popup(
        "RustAPI TestMod",
        "This popup was created from Rust code! The RustAPI bindings are working correctly.",
        "Nah",
        "Hell yeah",
        |clicked_btn2| {
            if clicked_btn2 {
                log_info("User thinks it's hell yeah cool!");
            } else {
                log_info("User is a hater");
            }
        }
    );
}

// Actual mod initialization
#[no_mangle] 
pub extern "C" fn testmod_init() {
    log_info("RustAPI TestMod loaded from Rust!");
    log_info("This mod is written entirely in Rust using RustAPI bindings");
}

#[no_mangle]
pub extern "C" fn testmod_shutdown() {
    log_info("rsapi testmod is killing itself");
}
