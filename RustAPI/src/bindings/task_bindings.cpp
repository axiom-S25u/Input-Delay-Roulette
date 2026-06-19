#include "helpers.hpp"

extern "C" {
    typedef void (*RustTaskCallback)(void*);

    void geode_task_run_main(RustTaskCallback cb, void* data) {
        geode::queueInMainThread([cb, data]() {
            if (cb) cb(data);
        });
    }
}
