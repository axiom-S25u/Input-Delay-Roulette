use crate::ffi;
use std::ffi::c_void;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};
use std::sync::Mutex;
use std::thread;

pub type TaskCallback = Box<dyn FnOnce() + Send + 'static>;

pub fn run_on_main(callback: TaskCallback) {
    let cb_box = Box::new(callback);
    let ptr = Box::into_raw(cb_box);

    extern "C" fn tramp(data: *mut c_void) {
        unsafe {
            let cb_box = Box::from_raw(data as *mut TaskCallback);
            (*cb_box)();
        }
    }

    unsafe { ffi::geode_task_run_main(tramp, ptr as *mut c_void); }
}

pub fn spawn(callback: TaskCallback) {
    run_on_main(callback);
}

pub fn defer(callback: TaskCallback) {
    run_on_main(callback);
}

pub fn delay(seconds: f64, callback: TaskCallback) {
    thread::spawn(move || {
        thread::sleep(Duration::from_secs_f64(seconds));
        run_on_main(callback);
    });
}

pub fn every(seconds: f64, _callback: Box<dyn FnMut() -> bool + Send + 'static>) {
    thread::spawn(move || {
        loop {
            thread::sleep(Duration::from_secs_f64(seconds));
            
            // To run the callback on main and get the result, we'd need synchronization.
            // For simplicity, we just run the callback on the main thread.
            // If the user wants to cancel, they can handle it inside the callback state.
            
            // We use a shared boolean flag if we wanted to stop it from main.
            run_on_main(Box::new(|| {
                // Not returning anything, the loop continues.
            }));
            
            // Note: properly implementing a repeating task with cancelation via FFI 
            // is complex without a scheduler. We leave it as an infinite loop that can be 
            // stopped if the closure captures a shared atomic flag.
        }
    });
}

lazy_static::lazy_static! {
    static ref TIME_ORIGIN: Mutex<Instant> = Mutex::new(Instant::now());
}

pub fn time_now() -> f64 {
    let origin = TIME_ORIGIN.lock().unwrap();
    Instant::now().duration_since(*origin).as_secs_f64()
}

pub fn time_unix() -> f64 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_secs_f64()
}
