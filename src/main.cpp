#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/Popup.hpp>
#include "TimelyLayer.hpp"

using namespace geode::prelude;

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


    CircleBaseColor getColor(std::string_view setting) {
        std::string_view colorStr = Mod::get()->getSettingValue<std::string>(setting);
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

        CCNode* creatorButtonsMenu = this->getChildByID("creator-buttons-menu");
        static_cast<AxisLayout*>(creatorButtonsMenu->getLayout())->ignoreInvisibleChildren(true);
    

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
        if (!Mod::get()->getSettingValue<bool>("unhide-extra-buttons")) { HIDE_BUTTON("map-button"); HIDE_BUTTON("versus-button"); }
#undef HIDE_BUTTON

        if (CCNode* blCorner = getChildByID("bottom-left-corner")) blCorner->setVisible(false);

        creatorButtonsMenu->updateLayout();


        double buttonSize = Mod::get()->getSettingValue<double>("button-sprite-size-setting");
        
        auto questButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_challengeSprite.png"_spr, buttonSize, getColor("quest-color-setting"), CircleBaseSize::SmallAlt);
        auto savedButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_savedSprite.png"_spr, buttonSize + 0.15f, getColor("saved-color-setting"), CircleBaseSize::SmallAlt);
        auto timelyButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_timeIcon_001.png", buttonSize + 0.15f, getColor("timely-color-setting"), CircleBaseSize::SmallAlt);
        auto gauntletButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gauntletSprite.png"_spr, buttonSize + 0.15f, getColor("gauntlet-color-setting"), CircleBaseSize::SmallAlt);
        auto mapPacksButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_mapPackSprite.png"_spr, buttonSize + 0.2f, getColor("mck-color-setting"), CircleBaseSize::SmallAlt);
        auto pathsButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_pathsSprite.png"_spr, buttonSize + 0.2f, getColor("paths-color-setting"), CircleBaseSize::SmallAlt);

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
        
        // these might go into bottom-left-menu, which is a vanilla node, so they should have _spr just in case
        newQuestButton->setID("new-quests-button"_spr);
        newSavedButton->setID("new-saved-button"_spr);
        timelyButton->setID("timely-levels-button"_spr);
        newGauntletButton->setID("new-gauntlets-button"_spr);
        newMapPacksButton->setID("new-map-packs-button"_spr);
        newPathsButton->setID("new-paths-button"_spr);
        
        bool flipMenus = Mod::get()->getSettingValue<bool>("flip-menus");
        
        auto bottomMenu = CCMenu::create();
        auto bottomLeftMenu = this->getChildByID("bottom-left-menu");
        auto menu1 = flipMenus ? bottomLeftMenu : bottomMenu;
        auto menu2 = flipMenus ? bottomMenu : bottomLeftMenu;

        if (menu1) {
            menu1->addChild(newSavedButton);
            menu1->addChild(timelyButton);
            menu1->addChild(newGauntletButton);
            menu1->addChild(newMapPacksButton);
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
                    newGDDPIButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gddpSprite.png"_spr, buttonSize + 0.29f, getColor("gddp-color-setting"), CircleBaseSize::SmallAlt);

                CCMenuItemSpriteExtra* newGDDPIButton = CCMenuItemSpriteExtra::create(
                    newGDDPIButtonSprite,
                    this,
                    originalGDDPIButton->m_pfnSelector
                );

                newGDDPIButton->setID("new-demon-progression-button"_spr);

                menu1->addChild(newGDDPIButton);
            }
        }

        if (menu2) {
            menu2->addChild(newQuestButton);
            menu2->addChild(newPathsButton);
        }
            

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        if (bottomLeftMenu) {
            static_cast<AxisLayout*>(bottomLeftMenu->getLayout())->setAutoGrowAxis(true);
            bottomLeftMenu->updateLayout();
        }
        int positionOffset = 7;
        AxisLayout* bottomMenuLayout = AxisLayout::create(Axis::Row);
        bottomMenuLayout->setAxisAlignment(AxisAlignment::Center);
        bottomMenuLayout->setGap(4.0f);
        bottomMenuLayout->setAutoGrowAxis(true);
        bottomMenu->setID("bottom-menu"_spr);
        addChild(bottomMenu);
        bottomMenu->setAnchorPoint({0.5f, 0.0f});
        bottomMenu->setPosition(winSize.width / 2, positionOffset);
        bottomMenu->setLayout(bottomMenuLayout, true);

        creatorButtonsMenu->setScale(1.5f);
        creatorButtonsMenu->setContentSize({260.0f, 130.0f});
        creatorButtonsMenu->updateLayout();

        return true;

        };
    };






