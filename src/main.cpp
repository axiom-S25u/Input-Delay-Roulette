#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include <deque>
#include <random>

using namespace geode::prelude;

struct input_info {
    bool down;
    int btn;
    bool p1;
    std::chrono::time_point<std::chrono::steady_clock> target;
};

class $modify(my_play_layer, PlayLayer) {
    struct Fields {
        std::deque<input_info> q;
        int val = 0;
        bool bypass = false;
        CCLabelBMFont* label = nullptr;
        std::mt19937 rng{std::random_device{}()};
    };

    bool init(GJGameLevel* level, bool use_replay, bool dont_save) {
        if (!PlayLayer::init(level, use_replay, dont_save)) return false;
        if (Mod::get()->getSettingValue<bool>("enabled")) this->m_isTestMode = true;
        auto sz = CCDirector::get()->getWinSize();
        m_fields->label = CCLabelBMFont::create("delay: 0ms", "goldFont.fnt");
        m_fields->label->setPosition({sz.width / 2, sz.height - 20.f});
        m_fields->label->setScale(0.5f);
        m_fields->label->setOpacity(255);
        this->addChild(m_fields->label, 9999);
        reroll();
        this->schedule(schedule_selector(my_play_layer::tick));
        return true;
    }

    void reroll() {
        int mn = (int)Mod::get()->getSettingValue<int64_t>("min-delay");
        int mx = (int)Mod::get()->getSettingValue<int64_t>("max-delay");
        if (mx < mn) mx = mn;
        m_fields->val = std::uniform_int_distribution<int>(mn, mx)(m_fields->rng);
        if (!m_fields->label) return;
        char buf[64];
        sprintf(buf, "delay: %dms", m_fields->val);
        m_fields->label->setString(buf);
        m_fields->label->stopAllActions();
        m_fields->label->setOpacity(255);
        m_fields->label->setScale(0.7f);
        m_fields->label->runAction(CCEaseBackOut::create(CCScaleTo::create(0.25f, 0.5f)));
    }

    void tick(float dt) {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        auto now = std::chrono::steady_clock::now();
        while (!m_fields->q.empty() && now >= m_fields->q.front().target) {
            auto info = m_fields->q.front();
            m_fields->q.pop_front();
            m_fields->bypass = true;
            GJBaseGameLayer::handleButton(info.down, info.btn, info.p1);
            m_fields->bypass = false;
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->q.clear();
        if (Mod::get()->getSettingValue<bool>("enabled")) {
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
        int delay = mpl->m_fields->val;
        if (down) mpl->reroll();
        mpl->m_fields->q.push_back({down, btn, p1,
            std::chrono::steady_clock::now() + std::chrono::milliseconds(delay)});
    }
};