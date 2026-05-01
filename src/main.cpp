#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include <deque>
#include <random>

using namespace geode::prelude;

static int g_rage_tax = 0;

struct input_info {
    bool down;
    int btn;
    bool p1;
    std::chrono::time_point<std::chrono::steady_clock> target;
    bool trigger_anxiety;
};

class $modify(my_play_layer, PlayLayer) {
    struct Fields {
        std::deque<input_info> q;
        int val = 0;
        bool bypass = false;
        CCLabelBMFont* label = nullptr;
        std::mt19937 rng{std::random_device{}()};
        int frames_alive = 0;
        float fake_input_timer = 0.f;
        float original_gravity = 0.f;
        bool in_anxiety = false;
    };

    bool init(GJGameLevel* level, bool use_replay, bool dont_save) {
        if (!PlayLayer::init(level, use_replay, dont_save)) return false;
        if (Mod::get()->getSettingValue<bool>("enabled")) this->m_isTestMode = true;
        
        auto sz = CCDirector::get()->getWinSize();
        m_fields->label = CCLabelBMFont::create("delay: 0ms", "goldFont.fnt");
        m_fields->label->setPosition({sz.width / 2, sz.height - 20.f});
        m_fields->label->setScale(0.5f);
        m_fields->label->setOpacity(255);
        int highestZ = 0;
        for (CCNode* child : CCArrayExt<CCNode*>(this->m_uiLayer->getChildren())) {
            if (child->getZOrder() > highestZ) highestZ = child->getZOrder();
        }
        this->m_uiLayer->addChild(m_fields->label, highestZ + 1);
        
        m_fields->fake_input_timer = std::uniform_real_distribution<float>(2.f, 10.f)(m_fields->rng);
        
        reroll();
        this->schedule(schedule_selector(my_play_layer::tick));
        return true;
    }

    void reroll() {
        if (!m_fields->label) return;
        
        int mn = (int)Mod::get()->getSettingValue<int64_t>("min-delay");
        int mx = (int)Mod::get()->getSettingValue<int64_t>("max-delay");
        if (mx < mn) mx = mn;

        int base_val = std::uniform_int_distribution<int>(mn, mx)(m_fields->rng);
        
        if (Mod::get()->getSettingValue<bool>("skill-penalty")) {
            base_val += m_fields->frames_alive / 120; 
        }

        m_fields->val = base_val + g_rage_tax;

        char buf[128];
        sprintf(buf, "delay: %dms", m_fields->val);

        m_fields->label->setString(buf);
        m_fields->label->stopAllActions();
        m_fields->label->setOpacity(255);
        m_fields->label->setScale(0.7f);
        m_fields->label->runAction(CCEaseBackOut::create(CCScaleTo::create(0.25f, 0.5f)));
    }

    void tick(float dt) {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        
        m_fields->frames_alive++;

        if (Mod::get()->getSettingValue<bool>("fake-input")) {
            m_fields->fake_input_timer -= dt;
            if (m_fields->fake_input_timer <= 0) {
                m_fields->fake_input_timer = std::uniform_real_distribution<float>(3.f, 15.f)(m_fields->rng);
                m_fields->bypass = true;
                GJBaseGameLayer::handleButton(true, 1, true);
                m_fields->bypass = false;
                
                auto target = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
                m_fields->q.push_back({false, 1, true, target, false});
            }
        }

        auto now = std::chrono::steady_clock::now();
        while (!m_fields->q.empty() && now >= m_fields->q.front().target) {
            auto info = m_fields->q.front();
            m_fields->q.pop_front();
            
            if (info.trigger_anxiety && info.down) {
                m_fields->in_anxiety = true;
                m_fields->original_gravity = m_player1->m_gravity;
                double strength = Mod::get()->getSettingValue<double>("jump-anxiety-strength");
                m_player1->m_gravity *= (float)strength;
                m_player2->m_gravity *= (float)strength;
            } else if (!info.down && m_fields->in_anxiety) {
                m_player1->m_gravity = m_fields->original_gravity;
                m_player2->m_gravity = m_fields->original_gravity;
                m_fields->in_anxiety = false;
            }

            m_fields->bypass = true;
            GJBaseGameLayer::handleButton(info.down, info.btn, info.p1);
            m_fields->bypass = false;
        }
    }

    void resetLevel() {
        if (m_fields->in_anxiety) {
            m_player1->m_gravity = m_fields->original_gravity;
            m_player2->m_gravity = m_fields->original_gravity;
            m_fields->in_anxiety = false;
        }
        PlayLayer::resetLevel();
        m_fields->q.clear();
        m_fields->frames_alive = 0;
        
        if (Mod::get()->getSettingValue<bool>("enabled")) {
            if (Mod::get()->getSettingValue<bool>("rage-tax")) {
                g_rage_tax += 10;
            }
            this->m_isTestMode = true;
            reroll();
        }
    }
};

class $modify(my_base, GJBaseGameLayer) {
    void handleButton(bool down, int btn, bool p1) {
        auto pl = PlayLayer::get();
        if (!pl || !Mod::get()->getSettingValue<bool>("enabled")) {
            GJBaseGameLayer::handleButton(down, btn, p1);
            return;
        }
        
        auto mpl = static_cast<my_play_layer*>(pl);
        if (mpl->m_fields->bypass) {
            GJBaseGameLayer::handleButton(down, btn, p1);
            return;
        }

        if (down && Mod::get()->getSettingValue<bool>("void-click")) {
            int rarity = (int)Mod::get()->getSettingValue<int64_t>("void-click-rarity");
            if (std::uniform_int_distribution<int>(0, 100)(mpl->m_fields->rng) < rarity) {
                return;
            }
        }

        if (down && Mod::get()->getSettingValue<bool>("input-hang-time")) {
            if (std::uniform_int_distribution<int>(0, 100)(mpl->m_fields->rng) < 5) {
                return; 
            }
        }

        bool anxiety = false;
        if (down && Mod::get()->getSettingValue<bool>("jump-anxiety") && btn == 1) {
            double chance = Mod::get()->getSettingValue<double>("jump-anxiety-chance");
            if (std::uniform_real_distribution<double>(0, 100)(mpl->m_fields->rng) < chance) {
                anxiety = true;
            }
        }

        int delay = mpl->m_fields->val;
        if (down) mpl->reroll();
        
        mpl->m_fields->q.push_back({down, btn, p1,
            std::chrono::steady_clock::now() + std::chrono::milliseconds(delay), anxiety});
    }
};
