#include "helpers.hpp"

extern "C" {
    void cocos_node_set_pos(void* node, float x, float y) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setPosition(x, y); 
    }
    void cocos_node_get_pos(void* node, float* x, float* y) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        auto p = n->getPosition(); *x = p.x; *y = p.y; 
    }
    void cocos_node_set_scale(void* node, float s) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setScale(s); 
    }
    float cocos_node_get_scale(void* node) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getScale(); 
    }
    void cocos_node_set_rot(void* node, float r) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setRotation(r); 
    }
    float cocos_node_get_rot(void* node) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getRotation(); 
    }
    void cocos_node_set_visible(void* node, bool v) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setVisible(v); 
    }
    bool cocos_node_is_visible(void* node) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->isVisible(); 
    }
    void cocos_node_set_opacity(void* node, uint8_t o) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        if (auto rgba = typeinfo_cast<cocos2d::CCRGBAProtocol*>(n)) rgba->setOpacity(o);
    }
    uint8_t cocos_node_get_opacity(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        if (auto rgba = typeinfo_cast<cocos2d::CCRGBAProtocol*>(n)) return rgba->getOpacity();
        return 255;
    }
    void cocos_node_set_color(void* node, uint8_t r, uint8_t g, uint8_t b) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        if (auto rgba = typeinfo_cast<cocos2d::CCRGBAProtocol*>(n)) {
            ccColor3B c; c.r = r; c.g = g; c.b = b;
            rgba->setColor(c);
        }
    }
    void cocos_node_get_color(void* node, uint8_t* r, uint8_t* g, uint8_t* b) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        if (auto rgba = typeinfo_cast<cocos2d::CCRGBAProtocol*>(n)) {
            auto c = rgba->getColor(); *r = c.r; *g = c.g; *b = c.b;
        } else {
            *r = *g = *b = 255;
        }
    }
    void cocos_node_set_content_size(void* node, float w, float h) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setContentSize({w, h});
    }
    void cocos_node_get_content_size(void* node, float* w, float* h) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        auto s = n->getContentSize();
        *w = s.width; *h = s.height;
    }
    void cocos_node_set_anchor(void* node, float x, float y) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setAnchorPoint({x, y});
    }
    void cocos_node_get_anchor(void* node, float* x, float* y) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        auto a = n->getAnchorPoint();
        *x = a.x; *y = a.y;
    }
    void cocos_node_set_z_order(void* node, int z) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setZOrder(z);
    }
    int cocos_node_get_z_order(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getZOrder();
    }
    int cocos_node_get_child_count(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getChildrenCount();
    }
    void cocos_node_set_id(void* node, const char* id) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->setID(id);
    }
    char* cocos_node_get_id(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        return alloc_string(n->getID());
    }
    void* cocos_node_get_child_by_id(void* node, const char* id) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getChildByID(id);
    }
    void geode_node_add_child(void* node, void* child) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        auto c = static_cast<cocos2d::CCNode*>(child);
        n->addChild(c); 
    }
    void geode_node_add_child_z(void* node, void* child, int z) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        auto c = static_cast<cocos2d::CCNode*>(child);
        n->addChild(c, z); 
    }
    void geode_node_remove_from_parent(void* node) { 
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->removeFromParent(); 
    }
    void geode_node_remove_all_children(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        n->removeAllChildren();
    }
    void* cocos_node_get_parent(void* node) {
        auto n = static_cast<cocos2d::CCNode*>(node);
        return n->getParent();
    }

    void geode_free_string(char* s) {
        if (s) free(s);
    }
}
