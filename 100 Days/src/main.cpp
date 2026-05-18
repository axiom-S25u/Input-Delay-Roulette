#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>
#include <matjson.hpp>
#include <fmt/format.h>
#include <ctime>
#include <thread>
#include <atomic>

using namespace geode::prelude;

// before you ping me saying the mod Streak! alr exists, its different
static char const* LVL_URL = "https://raw.githubusercontent.com/axiom-S25u/100dLvlID/main/level";

static bool inDestroyPlayer = false;

static std::string todayStr() {
    time_t tmp = time(nullptr);
    tm tInfo{};
#ifdef _WIN32
    localtime_s(&tInfo, &tmp);
#else
    localtime_r(&tmp, &tInfo);
#endif
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tInfo);
    return std::string(buf);
}

static int gimmeLvlId() {
    return (int)Mod::get()->getSavedValue<int64_t>("level-id", 0);
}

static matjson::Value gimmeDays() {
    matjson::Value val = Mod::get()->getSavedValue<matjson::Value>("days", matjson::Value::array());
    if (!val.isArray()) val = matjson::Value::array();
    return val;
}

static int howMany() {
    matjson::Value tmp = gimmeDays();
    return (int)tmp.size();
}

static bool gotToday(matjson::Value const& arr) {
    std::string td = todayStr();
    for (matjson::Value const& v : arr) {
        if (v.isString() && v.asString().unwrapOr("") == td) return true;
    }
    return false;
}

static void smashTodayIn() {
    matjson::Value arr = gimmeDays();
    if (gotToday(arr)) return;
    arr.push(todayStr());
    Mod::get()->setSavedValue<matjson::Value>("days", arr);
}

static int getHighZ(CCNode* node) {
    int z = 0;
    if (!node) return z;
    CCArray* kids = node->getChildren();
    if (!kids) return z;
    for (CCNode* c : CCArrayExt<CCNode*>(kids)) {
        if (c && c->getZOrder() > z) z = c->getZOrder();
    }
    return z;
}

static void boom(CCNode* parent) {
    if (!parent) return;
    CCSize sz = CCDirector::sharedDirector()->getWinSize();
    CCParticleExplosion* p = CCParticleExplosion::create();
    p->setPosition({sz.width / 2.f, sz.height / 2.f});
    p->setDuration(2.0f);
    p->setLife(2.0f);
    p->setTotalParticles(400);
    parent->addChild(p, getHighZ(parent) + 1);
    geode::utils::web::openLinkInBrowser("https://www.youtube.com/watch?v=dQw4w9WgXcQ"); // id say everyone knows this link, but dont click for ur safety
} // yes haha dead joke or smth idk

class $modify(NoclipDetectPre, PlayLayer) {
    static void onModify(auto& self) {
        (void) self.setHookPriority("PlayLayer::destroyPlayer", -0x800000);
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) override {
        if (object != m_anticheatSpike) inDestroyPlayer = true;
        PlayLayer::destroyPlayer(player, object);
        if (inDestroyPlayer) inDestroyPlayer = false;
    }
};

class LvlGrabber;

class MySexyPopup : public geode::Popup {
protected:
    GJGameLevel* m_lvl = nullptr;

    bool initSexy(GJGameLevel* lvl) {
        m_lvl = lvl;
        if (!Popup::init(400.f, 240.f)) return false;
        this->setTitle("100 Days");

        int days = howMany();
        if (days > 100) days = 100;

        CCLabelBMFont* lbl1 = CCLabelBMFont::create("Your Progress", "goldFont.fnt");
        lbl1->setScale(0.55f);
        lbl1->setPosition(85.f, 165.f);
        m_mainLayer->addChild(lbl1);

        CCLabelBMFont* lbl2 = CCLabelBMFont::create(fmt::format("{}/100", days).c_str(), "bigFont.fnt");
        lbl2->setScale(0.85f);
        lbl2->setPosition(85.f, 115.f);
        m_mainLayer->addChild(lbl2);

        CCLabelBMFont* lbl3 = CCLabelBMFont::create(fmt::format("{}%", days).c_str(), "bigFont.fnt");
        lbl3->setScale(0.5f);
        lbl3->setPosition(85.f, 85.f);
        m_mainLayer->addChild(lbl3);

        float ratio = (float)days / 100.f;

        CCLayerColor* bg = CCLayerColor::create({0, 0, 0, 180}, 140.f, 12.f);
        bg->ignoreAnchorPointForPosition(false);
        bg->setAnchorPoint({0.5f, 0.5f});
        bg->setPosition(85.f, 55.f);
        m_mainLayer->addChild(bg);

        if (ratio > 0.f) {
            CCLayerColor* fill = CCLayerColor::create({80, 220, 120, 230}, 140.f * ratio, 12.f);
            fill->ignoreAnchorPointForPosition(false);
            fill->setAnchorPoint({0.f, 0.5f});
            fill->setPosition(15.f, 55.f);
            m_mainLayer->addChild(fill);
        }

        CCLayerColor* line = CCLayerColor::create({255, 255, 255, 80}, 2.f, 180.f);
        line->setPosition(175.f, 25.f);
        m_mainLayer->addChild(line);

        std::string nm = m_lvl ? std::string(m_lvl->m_levelName.c_str()) : std::string("?");
        int stars = m_lvl ? (int)m_lvl->m_stars : 0;
        std::string desc = "(no description)";
        if (m_lvl) {
            gd::string rawDesc = m_lvl->m_levelDesc;
            if (!rawDesc.empty()) {
                gd::string dec = cocos2d::ZipUtils::base64URLDecode(rawDesc);
                if (!dec.empty()) desc = std::string(dec.c_str());
            }
        }

        CCLabelBMFont* nameLbl = CCLabelBMFont::create(nm.c_str(), "bigFont.fnt");
        nameLbl->setAnchorPoint({0.f, 0.5f});
        nameLbl->setPosition(195.f, 175.f);
        nameLbl->limitLabelWidth(150.f, 0.55f, 0.25f);
        m_mainLayer->addChild(nameLbl);

        CCSprite* starSpr = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
        if (starSpr) {
            starSpr->setScale(0.9f);
            starSpr->setPosition({380.f, 175.f});
            m_mainLayer->addChild(starSpr);
        }
        CCLabelBMFont* starLbl = CCLabelBMFont::create(std::to_string(stars).c_str(), "bigFont.fnt");
        starLbl->setScale(0.5f);
        starLbl->setAnchorPoint({1.f, 0.5f});
        starLbl->setPosition(368.f, 175.f);
        m_mainLayer->addChild(starLbl);

        CCLabelBMFont* descLbl = CCLabelBMFont::create(desc.c_str(), "chatFont.fnt");
        descLbl->setAnchorPoint({0.f, 1.f});
        descLbl->setPosition(195.f, 150.f);
        descLbl->setScale(0.65f);
        descLbl->setWidth(200);
        m_mainLayer->addChild(descLbl);

        CCSprite* playSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        if (!playSpr) playSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        playSpr->setScale(0.65f);
        CCMenuItemSpriteExtra* playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(MySexyPopup::onPlay));
        CCMenu* playMenu = CCMenu::create();
        playMenu->setPosition(290.f, 55.f);
        playMenu->addChild(playBtn);
        m_mainLayer->addChild(playMenu);

        ButtonSprite* checkSpr = ButtonSprite::create("Check Updates", "bigFont.fnt", "GJ_button_05.png", 0.7f);
        checkSpr->setScale(0.4f);
        CCMenuItemSpriteExtra* checkBtn = CCMenuItemSpriteExtra::create(checkSpr, this, menu_selector(MySexyPopup::onCheck));
        CCMenu* checkMenu = CCMenu::create();
        checkMenu->setPosition(290.f, 18.f);
        checkMenu->addChild(checkBtn);
        m_mainLayer->addChild(checkMenu);

        if (days >= 100) {
            ButtonSprite* reSpr = ButtonSprite::create("Restart", "bigFont.fnt", "GJ_button_06.png", 0.7f);
            reSpr->setScale(0.55f);
            CCMenuItemSpriteExtra* reBtn = CCMenuItemSpriteExtra::create(reSpr, this, menu_selector(MySexyPopup::onRestart));
            CCMenu* reMenu = CCMenu::create();
            reMenu->setPosition(85.f, 22.f);
            reMenu->addChild(reBtn);
            m_mainLayer->addChild(reMenu);
        }

        return true;
    }

    void onPlay(CCObject*) {
        if (!m_lvl) return;
        CCScene* sc = LevelInfoLayer::scene(m_lvl, false);
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, sc));
    }

    void onCheck(CCObject*);

    void onRestart(CCObject*) {
        geode::createQuickPopup(
            "Restart 100 Days",
            "wipe ur 100/100 and start over from 0? u sure",
            "Nah",
            "Wipe it",
            [this](FLAlertLayer*, bool yes) {
                if (!yes) return;
                Mod::get()->setSavedValue<matjson::Value>("days", matjson::Value::array());
                this->onClose(nullptr);
            }
        );
    }

public:
    static MySexyPopup* create(GJGameLevel* lvl) {
        MySexyPopup* tmp = new MySexyPopup();
        if (tmp && tmp->initSexy(lvl)) {
            tmp->autorelease();
            return tmp;
        }
        delete tmp;
        return nullptr;
    }
};

static std::atomic<bool> g_busy = false;

class LvlGrabber : public CCNode, public LevelManagerDelegate {
public:
    void killDelegate() {
        GameLevelManager* glm = GameLevelManager::sharedState();
        if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
    }

    static void openPopupFor(int idVal) {
        LvlGrabber* val = new LvlGrabber();
        val->autorelease();
        val->retain();
        GJSearchObject* srch = GJSearchObject::create(SearchType::Search, std::to_string(idVal));
        GameLevelManager* glm = GameLevelManager::sharedState();
        glm->m_levelManagerDelegate = val;
        glm->getOnlineLevels(srch);
    }

    static void grab() {
        if (g_busy.exchange(true)) return;
        std::thread([] {
            web::WebRequest req;
            web::WebResponse res = req.getSync(LVL_URL);
            int64_t parsedVal = 0;
            bool ok = false;
            if (res.ok()) {
                geode::Result<std::string> sr = res.string();
                if (sr) {
                    std::string raw = sr.unwrap();
                    size_t a = raw.find_first_not_of(" \t\r\n");
                    size_t b = raw.find_last_not_of(" \t\r\n");
                    if (a != std::string::npos) {
                        std::string trimmed = raw.substr(a, b - a + 1);
                        geode::Result<int64_t> parsed = geode::utils::numFromString<int64_t>(trimmed);
                        if (parsed) {
                            int64_t val = parsed.unwrap();
                            if (val > 0) {
                                parsedVal = val;
                                ok = true;
                            }
                        }
                    }
                }
            } else {
                log::warn("fetch dead (status {})", res.code());
            }
            Loader::get()->queueInMainThread([ok, parsedVal] {
                g_busy = false;
                if (!ok) {
                    FLAlertLayer::create("100 Days", "fetch died lmao try again later", "OK")->show();
                    return;
                }
                int64_t prev = Mod::get()->getSavedValue<int64_t>("level-id", 0);
                if (prev != parsedVal) {
                    Mod::get()->setSavedValue<int64_t>("level-id", parsedVal);
                    log::info("id changed {} -> {}", prev, parsedVal);
                }
                int idVal = (int)parsedVal;
                if (idVal <= 0) {
                    FLAlertLayer::create("100 Days", "level id aint loaded yet", "OK")->show();
                    return;
                }
                openPopupFor(idVal);
            });
        }).detach();
    }

    void loadLevelsFinished(CCArray* lvls, char const* key) override {
        loadLevelsFinished(lvls, key, 0);
    }
    void loadLevelsFinished(CCArray* lvls, char const*, int) override {
        killDelegate();
        if (!lvls || lvls->count() == 0) {
            FLAlertLayer::create("100 Days", "level not found bruh", "OK")->show();
            release();
            return;
        }
        GJGameLevel* lvl = typeinfo_cast<GJGameLevel*>(lvls->objectAtIndex(0));
        if (lvl) {
            CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
            if (scene) {
                CCArray* children = scene->getChildren();
                for (CCNode* child : CCArrayExt<CCNode*>(children)) {
                    MySexyPopup* oldPop = typeinfo_cast<MySexyPopup*>(child);
                    if (oldPop) oldPop->removeFromParentAndCleanup(true);
                }
            }
            MySexyPopup* pop = MySexyPopup::create(lvl);
            if (pop) pop->show();
        }
        release();
    }
    void loadLevelsFailed(char const* key) override { loadLevelsFailed(key, 0); }
    void loadLevelsFailed(char const*, int) override {
        killDelegate();
        FLAlertLayer::create("100 Days", "level fetch died lmao", "OK")->show();
        release();
    }
    void setupPageInfo(gd::string, char const*) override {}
};

void MySexyPopup::onCheck(CCObject*) {
    this->removeFromParentAndCleanup(true);
    LvlGrabber::grab();
}

class $modify(MyMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        int days = howMany();
        if (days > 100) days = 100;

        CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(
            CCSprite::create("logo.png"_spr),
            this,
            menu_selector(MyMenu::onBtn)
        );
        btn->setID("100days-button"_spr);

        CCLabelBMFont* lbl = CCLabelBMFont::create(fmt::format("{}/100", days).c_str(), "bigFont.fnt");
        lbl->setScale(0.4f);
        lbl->setPosition(btn->getContentSize().width / 2.f, -6.f);
        btn->addChild(lbl);

        CCNode* menu = this->getChildByID("bottom-menu");
        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onBtn(CCObject*) {
        LvlGrabber::grab();
    }
};

class $modify(MyPlay, PlayLayer) {
    struct Fields {
        bool m_isTarget = false;
        bool m_done = false;
        bool m_noclip = false;
    };

    bool init(GJGameLevel* lvl, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(lvl, useReplay, dontCreateObjects)) return false;
        int tgt = gimmeLvlId();
        if (tgt != 0 && lvl && (int)lvl->m_levelID == tgt) {
            m_fields->m_isTarget = true;
        }
        m_fields->m_noclip = false;
        return true;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) override {
        if (inDestroyPlayer) {
            m_fields->m_noclip = true;
        }
        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        PlayLayer::levelComplete();
        if (!m_fields->m_isTarget) return;
        if (m_fields->m_done) return;
        m_fields->m_done = true;

        if (m_fields->m_noclip) {
            FLAlertLayer::create("100 Days", "This wont count u cheater", "OK")->show(); // during testing i realised noclip worked bla blaaaa
            return;
        }

        bool wonBefore = howMany() >= 100;
        smashTodayIn();
        int nowVal = howMany();
        if (nowVal >= 100 && !wonBefore) {
            boom(this);
        }
    }
};
