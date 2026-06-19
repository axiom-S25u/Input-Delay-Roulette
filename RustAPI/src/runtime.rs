use std::collections::HashMap;
use std::thread;

use crate::ffi;

pub struct RustRuntime {
    scripts: HashMap<String, Script>,
    initialized: bool,
    main_thread_id: thread::ThreadId,
}

struct Script {
    source: String,
    compiled: bool,
}

impl RustRuntime {
    pub fn new() -> Self {
        Self {
            scripts: HashMap::new(),
            initialized: false,
            main_thread_id: thread::current().id(),
        }
    }

    pub fn init(&mut self) -> Result<(), String> {
        if self.initialized {
            return Ok(());
        }

        ffi::log_info("RustAPI runtime initializing...");
        
        self.initialized = true;
        ffi::log_info("RustAPI runtime initialized");
        Ok(())
    }

    pub fn shutdown(&mut self) {
        if !self.initialized {
            return;
        }

        ffi::log_info("RustAPI runtime shutting down...");
        self.scripts.clear();
        self.initialized = false;
    }

    pub fn is_initialized(&self) -> bool {
        self.initialized
    }

    pub fn is_main_thread(&self) -> bool {
        thread::current().id() == self.main_thread_id
    }

    pub fn run_script(&mut self, source: String, chunk_name: String) -> Result<(), String> {
        if !self.initialized {
            return Err("runtime not initialized".to_string());
        }

        let script = Script {
            source: source.clone(),
            compiled: false,
        };
        self.scripts.insert(chunk_name.clone(), script);

        ffi::log_info(&format!("RustAPI: would run script {}", chunk_name));
        Ok(())
    }

    pub fn run_file(&mut self, path: String) -> Result<(), String> {
        if !self.initialized {
            return Err("runtime not initialized".to_string());
        }

        ffi::log_info(&format!("RustAPI: would run file {}", path));
        Ok(())
    }
}
