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
        CCMenuItem* m_originalQuestButton = nullptr;
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

    void onNewSavedLevels(CCObject* sender) {
        CreatorLayer::onSavedLevels(sender);
    }

    void onTimely(CCObject*) {
        TimelyLayer::create(this)->show();
    }

    void onNewGauntlets(CCObject* sender) {
        CreatorLayer::onGauntlets(sender);
    }

    void onNewMapPacks(CCObject* sender) {
        CreatorLayer::onMapPacks(sender);
    }

    void onNewPaths(CCObject* sender) {
        CreatorLayer::onPaths(sender);
    }

    CircleBaseColor getColor(std::string colorStr) {
        CircleBaseColor buttonColor = CircleBaseColor::Green;
        if (colorStr == "Green") {
            buttonColor = CircleBaseColor::Green;
        }
        else if (colorStr == "Cyan")
        {
            buttonColor = CircleBaseColor::Cyan;
        }
        else if (colorStr == "Dark Aqua")
        {
            buttonColor = CircleBaseColor::DarkAqua;
        }
        else if (colorStr == "Dark Purple")
        {
            buttonColor = CircleBaseColor::DarkPurple;
        }
        else if (colorStr == "Gray")
        {
            buttonColor = CircleBaseColor::Gray;
        }
        else if (colorStr == "Pink")
        {
            buttonColor = CircleBaseColor::Pink;
        }
        else if (colorStr == "Blue")
        {
            buttonColor = CircleBaseColor::Blue;
        }

        return buttonColor;
    }

    bool init() {
        if (!CreatorLayer::init()) {
            return false;
        } else if (Mod::get()->getSettingValue<bool>("enable-setting")) {
            return true;
        }

        Mod::get()->setSavedValue("gddp-enabled", false);
        int positionOffset = 7;

		auto creatorButtonsMenu = this->getChildByID("creator-buttons-menu");
        auto layout = typeinfo_cast<AxisLayout*>(creatorButtonsMenu->getLayout());
        layout->ignoreInvisibleChildren(true);

        auto originalSavedButton = creatorButtonsMenu->getChildByID("saved-button");
        originalSavedButton->setVisible(false);
        originalSavedButton = nullptr;

		auto originalQuestButton = creatorButtonsMenu->getChildByID("quests-button");
        m_fields->m_originalQuestButton = typeinfo_cast<CCMenuItem*>(originalQuestButton);
        originalQuestButton->setVisible(false);
        originalQuestButton = nullptr;

        auto originalDailyButton = creatorButtonsMenu->getChildByID("daily-button");
        originalDailyButton->setVisible(false);
        originalDailyButton = nullptr;

        auto originalWeeklyButton = creatorButtonsMenu->getChildByID("weekly-button");
        originalWeeklyButton->setVisible(false);
        originalWeeklyButton = nullptr;

        auto originalEventButton = creatorButtonsMenu->getChildByID("event-button");
        originalEventButton->setVisible(false);
        originalEventButton = nullptr;

        auto originalGauntletButton = creatorButtonsMenu->getChildByID("gauntlets-button");
        originalGauntletButton->setVisible(false);
        originalGauntletButton = nullptr;

        auto originalMckButton = creatorButtonsMenu->getChildByID("map-packs-button");
        originalMckButton->setVisible(false);
        originalMckButton = nullptr;

        auto originalPathsButton = creatorButtonsMenu->getChildByID("paths-button");
        originalPathsButton->setVisible(false);
        originalPathsButton = nullptr;

        if (!Mod::get()->getSettingValue<bool>("unhide-extra-buttons")) {
            auto mapBtn = creatorButtonsMenu->getChildByID("map-button");
            mapBtn->setVisible(false);
            mapBtn = nullptr;

            auto vsBtn = creatorButtonsMenu->getChildByID("versus-button");
            vsBtn->setVisible(false);
            vsBtn = nullptr;
        }

		auto blCorner = this->getChildByID("bottom-left-corner");
		blCorner->setVisible(false);
        blCorner = nullptr;

        creatorButtonsMenu->updateLayout();

        auto questButtonColor = Mod::get()->getSettingValue<std::string>("quest-color-setting");
        CircleBaseColor questButtonColorV = getColor(questButtonColor);

        auto savedButtonColor = Mod::get()->getSettingValue<std::string>("saved-color-setting");
        CircleBaseColor savedButtonColorV = getColor(savedButtonColor);

        auto timelyButtonColor = Mod::get()->getSettingValue<std::string>("timely-color-setting");
        CircleBaseColor timelyButtonColorV = getColor(timelyButtonColor);

        auto gauntletButtonColor = Mod::get()->getSettingValue<std::string>("gauntlet-color-setting");
        CircleBaseColor gauntletButtonColorV = getColor(gauntletButtonColor);

        auto mckButtonColor = Mod::get()->getSettingValue<std::string>("mck-color-setting");
        CircleBaseColor mckButtonColorV = getColor(mckButtonColor);

        auto pathsButtonColor = Mod::get()->getSettingValue<std::string>("paths-color-setting");
        CircleBaseColor pathsButtonColorV = getColor(pathsButtonColor);

        auto questButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_challengeSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting"), questButtonColorV, CircleBaseSize::SmallAlt);
        auto savedButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_savedSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.15f, savedButtonColorV, CircleBaseSize::SmallAlt);
        auto timelyButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_timeIcon_001.png", Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.15f, timelyButtonColorV, CircleBaseSize::SmallAlt);
        auto gauntletButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gauntletSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.15f, gauntletButtonColorV, CircleBaseSize::SmallAlt);
        auto mckButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_mapPackSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.2f, mckButtonColorV, CircleBaseSize::SmallAlt);
        auto pathsButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_pathsSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.2f, pathsButtonColorV, CircleBaseSize::SmallAlt);

        auto newQuestButton = CCMenuItemSpriteExtra::create(
            questButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewChallenge)
        );

        auto newSavedButton = CCMenuItemSpriteExtra::create(
            savedButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewSavedLevels)
        );

        auto timelyButton = CCMenuItemSpriteExtra::create(
            timelyButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onTimely)
        );

        auto newGauntletButton = CCMenuItemSpriteExtra::create(
            gauntletButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewGauntlets)
        );

        auto newMckButton = CCMenuItemSpriteExtra::create(
            mckButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewMapPacks)
        );

        auto newPathsButton = CCMenuItemSpriteExtra::create(
            pathsButtonSprite,
            this,
            menu_selector(BetterCreatorLayer::onNewPaths)
        );

        newQuestButton->setID("new-quests-button");
        newSavedButton->setID("new-saved-button");
        timelyButton->setID("timely-levels-button");
        newGauntletButton->setID("new-gauntlets-button");
        newMckButton->setID("new-map-packs-button");
        newPathsButton->setID("new-paths-button");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (auto bottomLeftMenu = this->getChildByID("bottom-left-menu")) {
            if (Mod::get()->getSettingValue<bool>("flip-menus")) {
    		    bottomLeftMenu->addChild(newSavedButton);
                bottomLeftMenu->addChild(timelyButton);
                bottomLeftMenu->addChild(newGauntletButton);
                bottomLeftMenu->addChild(newMckButton);
                if (Loader::get()->isModLoaded("minemaker0430.gddp_integration")) {
                    Mod::get()->setSavedValue("gddp-enabled", true);

                    auto originalGddpNode = creatorButtonsMenu->getChildByID("demon-progression-button");
                    auto originalGddpButton = static_cast<CCMenuItemSpriteExtra*>(originalGddpNode);
                    originalGddpButton->setVisible(false);

                    auto gddpButtonColor = Mod::get()->getSettingValue<std::string>("gddp-color-setting");
                    CircleBaseColor gddpButtonColorV = getColor(gddpButtonColor);

                    CircleButtonSprite* gddpButtonSprite;

                    if (!Mod::get()->getSettingValue<bool>("stupid-gddp-sprite")) {
                        gddpButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gddpSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.29f, gddpButtonColorV, CircleBaseSize::SmallAlt);
                    } else {
                        gddpButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_stupidGddpSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.25f, gddpButtonColorV, CircleBaseSize::SmallAlt);
                    }

                    auto newGddpButton = CCMenuItemSpriteExtra::create(
                        gddpButtonSprite,
                        this,
                        originalGddpButton->m_pfnSelector
                    );

                    newGddpButton->setID("new-demon-progression-button");

                    bottomLeftMenu->addChild(newGddpButton);
                }
            } else {
                bottomLeftMenu->addChild(newQuestButton);
                bottomLeftMenu->addChild(newPathsButton);
            }

            AxisLayout* bLMLayout = static_cast<AxisLayout*>(bottomLeftMenu->getLayout());
            bLMLayout->setAutoGrowAxis(true);
		    bottomLeftMenu->updateLayout();
	    }
		
        AxisLayout* bottomMenuLayout = AxisLayout::create(Axis::Row);
        bottomMenuLayout->setAxisAlignment(AxisAlignment::Center);
        bottomMenuLayout->setGap(4.0f);
        bottomMenuLayout->setAutoGrowAxis(true);

        auto bottomMenu = CCMenu::create();
        bottomMenu->setID("bottom-menu"_spr);

        if (Mod::get()->getSettingValue<bool>("flip-menus")) {
            bottomMenu->addChild(newQuestButton);
            bottomMenu->addChild(newPathsButton);
        } else {
            bottomMenu->addChild(newSavedButton);
            bottomMenu->addChild(timelyButton);
            bottomMenu->addChild(newGauntletButton);
            bottomMenu->addChild(newMckButton);
            if (Loader::get()->isModLoaded("minemaker0430.gddp_integration")) {
                Mod::get()->setSavedValue("gddp-enabled", true);
                auto originalGddpNode = creatorButtonsMenu->getChildByID("demon-progression-button");
                auto originalGddpButton = static_cast<CCMenuItemSpriteExtra*>(originalGddpNode);

                originalGddpButton->setVisible(false);

                auto gddpButtonColor = Mod::get()->getSettingValue<std::string>("gddp-color-setting");
                CircleBaseColor gddpButtonColorV = getColor(gddpButtonColor);

                CircleButtonSprite* gddpButtonSprite;

                if (!Mod::get()->getSettingValue<bool>("stupid-gddp-sprite")) {
                    gddpButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_gddpSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.29f, gddpButtonColorV, CircleBaseSize::SmallAlt);
                } else {
                    gddpButtonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_stupidGddpSprite.png"_spr, Mod::get()->getSettingValue<double>("button-sprite-size-setting") + 0.25f, gddpButtonColorV, CircleBaseSize::SmallAlt);
                }

                auto newGddpButton = CCMenuItemSpriteExtra::create(
                    gddpButtonSprite,
                    this,
                    originalGddpButton->m_pfnSelector
                );

                newGddpButton->setID("new-demon-progression-button");
                bottomMenu->addChild(newGddpButton);
            }
        }

        this->addChild(bottomMenu);

        bottomMenu->setAnchorPoint({0.5f, 0.0f});
        bottomMenu->setPosition(winSize.width / 2, positionOffset);
        bottomMenu->setLayout(bottomMenuLayout, true);

        creatorButtonsMenu->setScale(1.5f);
        creatorButtonsMenu->setContentSize({260.0f, 130.0f});
        creatorButtonsMenu->updateLayout();

        return true;

        };
    };



