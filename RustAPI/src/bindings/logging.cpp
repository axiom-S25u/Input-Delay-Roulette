#include "helpers.hpp"

extern "C" {
    void geode_log_info(const char* msg) { log::info("{}", msg); }
    void geode_log_warn(const char* msg) { log::warn("{}", msg); }
    void geode_log_error(const char* msg) { log::error("{}", msg); }
}
