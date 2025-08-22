// Geode Header
#include <Geode/Geode.hpp>

// Other Headers
#include <Geode/modify/CreatorLayer.hpp>

#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/Popup.hpp>

#include <Geode/utils/cocos.hpp>

// Include Timely Levels Layer         
#include "TimelyLayer.hpp"

// Geode Namespace
using namespace geode::prelude;

// Class for Quest Notification
class QuestNotification final : public CCSprite {
public:
	static QuestNotification* createWithSpriteFrameName(char const* frameName) {
		auto ret = new QuestNotification;
		if (ret -> initWithSpriteFrameName(frameName)) {
			ret -> autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
	void setQuestsSprite(CCSprite* questsSprite) {
		m_questsSprite = questsSprite;

		return;
	}
private:
	CCSprite* m_questsSprite;

	void update(float dt) override {
		this -> setVisible(m_questsSprite -> getChildrenCount());

		return;
	}
};

// Main Code
class $modify(BetterCreatorLayer, CreatorLayer) {

    static void onModify(auto& self) {
        if (!self.setHookPriorityAfterPost("CreatorLayer::init", "minemaker0430.gddp_integration")) {
            log::warn("Failed to set hook priority.");
        }
    }

    struct Fields {
        CCMenuItemSpriteExtra* m_originalQuestButton = nullptr;
    };

    void onExit() {
        if (m_fields->m_originalQuestButton) {
            m_fields->m_originalQuestButton->release();
            m_fields->m_originalQuestButton = nullptr;
        }
        CreatorLayer::onExit();
    }

    void onNewChallenge(CCObject* sender) {
        CreatorLayer::onChallenge(m_fields->m_originalQuestButton);
    }

    void onTimely(CCObject*) {
        TimelyLayer::create(this)->show();
    }

    CircleBaseColor getColor(std::string setting) {
        std::string colorStr = Mod::get()->getSettingValue<std::string>(setting);
        if      (colorStr == "Green")       return CircleBaseColor::Green;
        else if (colorStr == "Cyan")        return CircleBaseColor::Cyan;
        else if (colorStr == "Dark Aqua")   return CircleBaseColor::DarkAqua;
        else if (colorStr == "Dark Purple") return CircleBaseColor::DarkPurple;
        else if (colorStr == "Gray")        return CircleBaseColor::Gray;
        else if (colorStr == "Pink")        return CircleBaseColor::Pink;
        else if (colorStr == "Blue")        return CircleBaseColor::Blue;
        else                                return CircleBaseColor::Green;
    }

    bool init() {
        if (!CreatorLayer::init()) {
            return false;
        } else if (Mod::get()->getSettingValue<bool>("enable-setting")) {
            return true;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        CCNode* creatorButtonsMenu = this->getChildByID("creator-buttons-menu");
        static_cast<AxisLayout*>(creatorButtonsMenu->getLayout())->ignoreInvisibleChildren(true);
    
        auto ogQuestBtn = creatorButtonsMenu->getChildByID("quests-button");
        m_fields->m_originalQuestButton = static_cast<CCMenuItemSpriteExtra*>(creatorButtonsMenu->getChildByID("quests-button"));
        m_fields->m_originalQuestButton->setVisible(false);

        #define HIDE_BUTTON(name) if (CCNode* originalButton = creatorButtonsMenu->getChildByID(name)) originalButton->setVisible(false)
                HIDE_BUTTON("saved-button");
                HIDE_BUTTON("daily-button");
                HIDE_BUTTON("weekly-button");
                HIDE_BUTTON("event-button");
                HIDE_BUTTON("gauntlets-button");
                HIDE_BUTTON("map-packs-button");
                HIDE_BUTTON("paths-button");
                HIDE_BUTTON("scores-button");
                if (!Mod::get()->getSettingValue<bool>("unhide-extra-buttons")) { HIDE_BUTTON("map-button"); HIDE_BUTTON("versus-button"); }
        #undef HIDE_BUTTON

        if (CCNode* blCorner = getChildByID("bottom-left-corner")) blCorner->setVisible(false);

        creatorButtonsMenu->updateLayout();

        double buttonSize = Mod::get()->getSettingValue<double>("button-sprite-size-setting");
        
        auto questButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_challengeSprite.png"_spr, buttonSize, getColor("quest-color-setting"), CircleBaseSize::SmallAlt);
        auto savedButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_savedSprite.png"_spr, buttonSize + 0.15f, getColor("saved-color-setting"), CircleBaseSize::SmallAlt);
        CCSprite* timelyButtonSprite; 
        if (!Mod::get()->getSettingValue<bool>("compact-button")) {
            timelyButtonSprite = CategoryButtonSprite::createWithSpriteFrameName("GJ_timelySprite_002.png"_spr, 0.877f, CategoryBaseColor::Green, CategoryBaseSize::Big);
            timelyButtonSprite->setScale(0.8f);
        } else {
            timelyButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_timelySprite_001.png"_spr, buttonSize + 0.15f, getColor("timely-color-setting"), CircleBaseSize::SmallAlt);
        }
        auto gauntletButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gauntletSprite.png"_spr, buttonSize + 0.15f, getColor("gauntlet-color-setting"), CircleBaseSize::SmallAlt);
        auto mapPacksButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_mapPackSprite.png"_spr, buttonSize + 0.2f, getColor("mck-color-setting"), CircleBaseSize::SmallAlt);
        auto pathsButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_pathsSprite.png"_spr, buttonSize + 0.2f, getColor("paths-color-setting"), CircleBaseSize::SmallAlt);
        auto scoresButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_scoresSprite.png"_spr, buttonSize + 0.2f, getColor("scores-color-setting"), CircleBaseSize::SmallAlt);
        
        auto newQuestButton = CCMenuItemSpriteExtra::create(
            questButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewChallenge)
        );

        auto newSavedButton = CCMenuItemSpriteExtra::create(
            savedButtonSprite,
            this,
            menu_selector(CreatorLayer::onSavedLevels)
        );

        auto timelyButton = CCMenuItemSpriteExtra::create(
            timelyButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onTimely)
        );

        if (!Mod::get()->getSettingValue<bool>("compact-button")) {
            timelyButton->setContentSize({ 83.6f, 83.6f });
		    timelyButton->setPosition({ 450.f, 155.f });
            typeinfo_cast<CCSprite*>(timelyButton->getChildren()->objectAtIndex(0))->setPositionX(44.f);
        }

        auto newGauntletButton = CCMenuItemSpriteExtra::create(
            gauntletButtonSprite,
            this,
            menu_selector(CreatorLayer::onGauntlets)
        );

        auto newMapPacksButton = CCMenuItemSpriteExtra::create(
            mapPacksButtonSprite,
            this,
            menu_selector(CreatorLayer::onMapPacks)
        );

        auto newPathsButton = CCMenuItemSpriteExtra::create(
            pathsButtonSprite,
            this,
            menu_selector(CreatorLayer::onPaths)
        );

        auto newScoresButton = CCMenuItemSpriteExtra::create(
            scoresButtonSprite,
            this,
            menu_selector(CreatorLayer::onLeaderboards)
        );
        
        // These might go into bottom-left-menu, which is a vanilla node, so they should have _spr just in case
        newQuestButton->setID("new-quests-button"_spr);
        newSavedButton->setID("new-saved-button"_spr);
        timelyButton->setID("timely-levels-button"_spr);
        newGauntletButton->setID("new-gauntlets-button"_spr);
        newMapPacksButton->setID("new-map-packs-button"_spr);
        newPathsButton->setID("new-paths-button"_spr);
        newScoresButton->setID("new-scores-button");

		auto questNotif = QuestNotification::createWithSpriteFrameName("exMark_001.png");
		
		questNotif->setPosition(newQuestButton->getContentSize() - ccp(8.0f,9.0f));
        questNotif->setScale(0.55f);
        questNotif->setQuestsSprite(m_questsSprite);
		questNotif->scheduleUpdate();
		newQuestButton->addChild(questNotif);
		questNotif->setID("quest-notification");

        bool flipMenus = Mod::get()->getSettingValue<bool>("flip-menus");
        
        auto bottomMenu = CCMenu::create();
        auto bottomLeftMenu = this->getChildByID("bottom-left-menu");
        auto rightMenu = this->getChildByID("cvolton.betterinfo/center-right-menu");
        if (!rightMenu) rightMenu = this->getChildByID("minemaker0430.gddp_integration/center-right-menu");
        if (!rightMenu) {
            rightMenu = CCMenu::create();
            rightMenu->setID("center-right-menu"_spr);
        } 
        auto menu1 = flipMenus ? bottomLeftMenu : bottomMenu;
        auto menu2 = flipMenus ? bottomMenu : bottomLeftMenu;

        int positionOffset = 7;

        if (menu1) {
            menu1->addChild(newSavedButton);
            if (Mod::get()->getSettingValue<bool>("compact-button")) {   
                menu1->addChild(timelyButton);
            }
            menu1->addChild(newGauntletButton);
            menu1->addChild(newMapPacksButton);
            if (Loader::get()->isModLoaded("minemaker0430.gddp_integration")) {
                // option 1: non-compact button
                CCMenuItemSpriteExtra* originalGDDPIButton = static_cast<CCMenuItemSpriteExtra*>(creatorButtonsMenu->getChildByID("demon-progression-button"));
                CCNode* centerRightMenu = nullptr;
                // option 2: compact button
                if (!originalGDDPIButton) {
                    centerRightMenu = getChildByID("minemaker0430.gddp_integration/center-right-menu");
                    if (!centerRightMenu) centerRightMenu = getChildByID("cvolton.betterinfo/center-right-menu");
                    originalGDDPIButton = static_cast<CCMenuItemSpriteExtra*>(centerRightMenu->getChildByID("demon-progression-button"));
                }
                if (originalGDDPIButton) {
                    originalGDDPIButton->setVisible(false);
                    if (centerRightMenu) {
                      geode::Layout* layout = centerRightMenu->getLayout();
                      if (layout) layout->ignoreInvisibleChildren(true);
                      centerRightMenu->updateLayout();
                    }
                    CircleButtonSprite* newGDDPIButtonSprite;
                    if (Mod::get()->getSettingValue<bool>("stupid-gddp-sprite"))
                        newGDDPIButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_stupidGddpSprite.png"_spr, buttonSize + 0.25f, getColor("gddp-color-setting"), CircleBaseSize::SmallAlt);
                    else
                        newGDDPIButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gddpSprite.png"_spr, buttonSize + 0.3f, getColor("gddp-color-setting"), CircleBaseSize::SmallAlt);

                    CCMenuItemSpriteExtra* newGDDPIButton = CCMenuItemSpriteExtra::create(
                        newGDDPIButtonSprite,
                        originalGDDPIButton->m_pListener,
                        originalGDDPIButton->m_pfnSelector
                    );

                    newGDDPIButton->setID("new-demon-progression-button"_spr);

                    menu1->addChild(newGDDPIButton);
                }
            }
            if (Loader::get()->isModLoaded("spaghettdev.gd-roulette")) {
                CCMenuItemSpriteExtra* originalGDRButton = static_cast<CCMenuItemSpriteExtra*>(creatorButtonsMenu->getChildByID("spaghettdev.gd-roulette/roulette-button"));
                CCNode* centerLeftMenu = nullptr;

                if (!originalGDRButton) {
                    centerLeftMenu = getChildByID("spaghettdev.gd-roulette/center-left-menu");
                    originalGDRButton = static_cast<CCMenuItemSpriteExtra*>(centerLeftMenu->getChildByID("spaghettdev.gd-roulette/roulette-button"));
                }
                if (originalGDRButton) {
                    originalGDRButton->setVisible(false);

                    auto newGDRButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_GDRSprite.png"_spr, buttonSize + 0.25f, getColor("gdr-color-setting"), CircleBaseSize::SmallAlt);

                    CCMenuItemSpriteExtra* newGDRButton = CCMenuItemSpriteExtra::create(
                        newGDRButtonSprite,
                        originalGDRButton->m_pListener,
                        originalGDRButton->m_pfnSelector
                    );

                    newGDRButton->setID("new-roulette-button"_spr);

                    menu1->addChild(newGDRButton);
                }
            }
        }

        if (menu2) {
            menu2->addChild(newQuestButton);
            menu2->addChild(newPathsButton);
        }

        if (rightMenu) {
            rightMenu->addChild(newScoresButton);
            if (Loader::get()->isModLoaded("khronophobia.shortcuts_menu")) {
                auto bottomRightMenu = this->getChildByID("bottom-right-menu");
                CCMenuItemSpriteExtra* ogShortcutsBtn = static_cast<CCMenuItemSpriteExtra*>(bottomRightMenu->getChildByID("khronophobia.shortcuts_menu/shortcuts-menu-button"));

                if (ogShortcutsBtn) {
                    ogShortcutsBtn->setVisible(false);
                    bottomRightMenu->getLayout()->ignoreInvisibleChildren(true);
                    bottomRightMenu->updateLayout();

                    auto newSCBtnSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_scSprite.png"_spr, buttonSize, getColor("sc-color-setting"), CircleBaseSize::SmallAlt);

                    auto newSCButton = CCMenuItemSpriteExtra::create(
                        newSCBtnSprite, 
                        ogShortcutsBtn->m_pListener, 
                        ogShortcutsBtn->m_pfnSelector
                    );

                    newSCButton->setID("new-shortcuts-menu-button"_spr);

                    rightMenu->addChild(newSCButton);
                }
            }
            if (Loader::get()->isModLoaded("techstudent10.gdguesser")) {
                auto bottomRightMenu = this->getChildByID("bottom-right-menu");
                CCMenuItemSpriteExtra* ogGDGBtn = static_cast<CCMenuItemSpriteExtra*>(bottomRightMenu->getChildByID("techstudent10.gdguesser/start-btn"));

                if (ogGDGBtn) {
                    ogGDGBtn->setVisible(false);
                    bottomRightMenu->getLayout()->ignoreInvisibleChildren(true);
                    bottomRightMenu->updateLayout();

                    auto newGDGBtnSprite = CCSprite::createWithSpriteFrameName("GJ_gdgSprite.png"_spr);
                    newGDGBtnSprite->setScale(0.101f);

                    auto newGDGButton = CCMenuItemSpriteExtra::create(
                        newGDGBtnSprite, 
                        ogGDGBtn->m_pListener, 
                        ogGDGBtn->m_pfnSelector
                    );

                    newGDGButton->setID("new-guesser-start-button"_spr);

                    rightMenu->addChild(newGDGButton);
                }
            }

            AxisLayout* rightMenuLayout = AxisLayout::create(Axis::Column);
            rightMenuLayout->setAxisAlignment(AxisAlignment::Center);
            rightMenuLayout->setGap(4.0f);
            rightMenuLayout->ignoreInvisibleChildren(true);
            if (rightMenu->getID() == "center-right-menu"_spr) this->addChild(rightMenu);
            rightMenu->setAnchorPoint({1.0f, 0.5f});
            rightMenu->setPosition(winSize.width -4.f, winSize.height / 2.f);
            rightMenu->setContentHeight(210.0f);
            rightMenu->setLayout(rightMenuLayout, true);
        }
        
        if (bottomLeftMenu) {
            bottomLeftMenu->setContentHeight(240.0f);
            bottomLeftMenu->updateLayout();
        }

        AxisLayout* bottomMenuLayout = AxisLayout::create(Axis::Row);
        bottomMenuLayout->setAxisAlignment(AxisAlignment::Center);
        bottomMenuLayout->setGap(4.0f);
        bottomMenu->setID("bottom-menu"_spr);
        addChild(bottomMenu);
        bottomMenu->setAnchorPoint({0.5f, 0.0f});
        bottomMenu->setPosition(winSize.width / 2, positionOffset);
        bottomMenu->setContentWidth(430.0f);
        bottomMenu->setLayout(bottomMenuLayout, true);

        // Changes to creator-buttons-menu
        creatorButtonsMenu->setScale(1.5f);
        creatorButtonsMenu->setContentSize({230.0f, 160.0f});
        creatorButtonsMenu->setPosition({winSize.width / 2, creatorButtonsMenu->getPositionY() + 20.0f});

        auto featuredBtn = creatorButtonsMenu->getChildByID("featured-button");
        auto searchBtn = creatorButtonsMenu->getChildByID("search-button");
        auto createBtn = creatorButtonsMenu->getChildByID("create-button");
        auto listsBtn = creatorButtonsMenu->getChildByID("lists-button");
        auto mapBtn = creatorButtonsMenu->getChildByID("map-button");
        auto versusBtn = creatorButtonsMenu->getChildByID("versus-button");

        featuredBtn->setZOrder(-10);
        searchBtn->setZOrder(1);
        createBtn->setZOrder(5);
        listsBtn->setZOrder(10);
        mapBtn->setZOrder(10);
        versusBtn->setZOrder(15);

        if (!Mod::get()->getSettingValue<bool>("compact-button")) {
            creatorButtonsMenu->addChild(timelyButton);
            timelyButton->setZOrder(-5);
            if (Mod::get()->getSettingValue<bool>("unhide-extra-buttons")) {
                creatorButtonsMenu->setContentWidth(290.f);

                featuredBtn->setZOrder(-15);
                listsBtn->setZOrder(-10);
            }
        } else if (Mod::get()->getSettingValue<bool>("unhide-extra-buttons")) {
            listsBtn->setZOrder(-5);
        }

        creatorButtonsMenu->updateLayout();

        return true;

        };
    };

