#include "helpers.hpp"

extern "C" {
    char* geode_keybind_from_string(const char* str) {
        auto res = Keybind::fromString(str);
        if (res.isErr()) return nullptr;
        auto kb = res.unwrap();
        matjson::Value obj = matjson::Value::object();
        obj.set("key", static_cast<int>(kb.key));
        obj.set("modifiers", static_cast<int>(kb.modifiers));
        return alloc_string(obj.dump());
    }
    char* geode_keybind_to_string(int key, int modifiers) {
        Keybind kb(static_cast<cocos2d::enumKeyCodes>(key), static_cast<KeyboardModifier>(modifiers));
        return alloc_string(kb.toString());
    }

    typedef void (*RustWebCallback)(int status, const char* body, const char* error);

    void geode_web_fetch(const char* method, const char* url, const char* body, RustWebCallback cb) {
        std::string m = method;
        std::string u = url;
        std::string b = body ? body : "";
        
        geode::utils::coro::spawn << [m, u, b, cb]() -> geode::Task<void> {
            auto req = utils::web::WebRequest();
            if (!b.empty()) req.bodyString(b);
            
            auto res = co_await req.send(m, u);
            if (res.ok()) {
                auto data = res.string().unwrapOr("");
                char* bodyCopy = alloc_string(data);
                cb(res.code(), bodyCopy, nullptr);
            } else {
                auto err = std::to_string(res.code());
                cb(res.code(), nullptr, err.c_str());
            }
        };
    }
    void geode_web_open_link(const char* url) {
        utils::web::openLinkInBrowser(url);
    }
}
