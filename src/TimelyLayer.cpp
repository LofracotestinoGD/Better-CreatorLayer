#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "TimelyLayer.hpp"

using namespace geode::prelude;

bool TimelyLayer::setup(CreatorLayer* creatorLayer) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->setID("TimelyLayer");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
  
    m_creatorLayer = creatorLayer;
 
    auto dailyLvlSpr = CCSprite::createWithSpriteFrameName("GJ_dailyTitle.png"_spr);
    auto weeklyLvlSpr = CCSprite::createWithSpriteFrameName("GJ_weeklyTitle.png"_spr);
    auto eventLvlSpr = CCSprite::createWithSpriteFrameName("GJ_eventTitle.png"_spr);

    auto dailyLvlBtnSpr = ButtonSprite::create(dailyLvlSpr, 230, 0, 37.f, 1.f, true, "RCL_button_08.png"_spr, false);

    auto weeklyLvlBtnSpr = ButtonSprite::create(weeklyLvlSpr, 285, 1, 0.f, 1.f, true, "RCL_button_07.png"_spr, false);

    auto eventLvlBtnSpr = ButtonSprite::create(eventLvlSpr, 245, 0, 0.f, 1.f, true, "RCL_button_06.png"_spr, false);

    auto newDailyBtn = CCMenuItemSpriteExtra::create(
        dailyLvlBtnSpr,
        this,
        menu_selector(TimelyLayer::onNewDaily)
    );

    auto newWeeklyBtn = CCMenuItemSpriteExtra::create(
        weeklyLvlBtnSpr,
        this,
        menu_selector(TimelyLayer::onNewWeekly)
    );

    auto newEventBtn = CCMenuItemSpriteExtra::create(
        eventLvlBtnSpr,
        this,
        menu_selector(TimelyLayer::onNewEvent)
    );

    auto infoBtn = InfoAlertButton::create("Timely Levels", "In Geometry Dash, there are three special featured levels: <cy>Daily</c>, <cj>Weekly</c>, and <ca>Event Levels</c>. The <cy>Daily Level</c> is a user-made level picked daily by RobTop, with varying difficulty and rewards like diamonds and orbs. The <cj>Weekly Demon</c> is a tougher level released weekly, always rated Demon, and offers bigger rewards like demon keys. <ca>Event Levels</c> appear on special occasions, often from top creators or collabs, with any difficulty and unique rewards like vault keys or exclusive icons.", 1.0f);
    infoBtn->setID("info-button");

    auto artSpr01 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    artSpr01->setID("timely-bottom-left-art");
    artSpr01->setAnchorPoint({0.0f, 0.0f});


    auto artSpr02 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    artSpr02->setID("timely-bottom-right-art");
    artSpr02->setFlipX(true);
    artSpr02->setAnchorPoint({1.0f, 0.0f});

    auto artSpr03 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    artSpr03->setID("timely-top-left-art");
    artSpr03->setFlipY(true);
    artSpr03->setAnchorPoint({0.0f, 1.0f});

    auto artSpr04 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    artSpr04->setID("timely-top-right-art");
    artSpr04->setFlipX(true);
    artSpr04->setFlipY(true);
    artSpr04->setAnchorPoint({1.0f, 1.0f});

    auto titleSpr = CCSprite::createWithSpriteFrameName("TimelyLevelsLogo.png"_spr);

    AxisLayout* buttonMenuLayout = AxisLayout::create(Axis::Column);
    buttonMenuLayout->setAxisAlignment(AxisAlignment::Start);
    buttonMenuLayout->setAxisAlignment(AxisAlignment::Even);
    buttonMenuLayout->setAxisReverse(true);

    auto buttonMenu = CCMenu::create();
    buttonMenu->addChild(newDailyBtn);
    buttonMenu->addChild(newWeeklyBtn);
    buttonMenu->addChild(newEventBtn);
    buttonMenu->setContentHeight(185.0f);
    buttonMenu->setID("timely-buttons-menu");
    m_mainLayer->addChildAtPosition(buttonMenu, Anchor::Center, {0.0f, -15.0f});
    buttonMenu->setLayout(buttonMenuLayout, true);

    auto artMenu = CCMenu::create();
    artMenu->setContentSize({420.0f, 280.0f});
    artMenu->setID("art-menu");
    artMenu->addChildAtPosition(artSpr01, Anchor::BottomLeft);
    artMenu->addChildAtPosition(artSpr02, Anchor::BottomRight);
    artMenu->addChildAtPosition(artSpr03, Anchor::TopLeft);
    artMenu->addChildAtPosition(artSpr04, Anchor::TopRight);
    m_mainLayer->addChildAtPosition(artMenu, Anchor::Center);

    m_mainLayer->addChildAtPosition(titleSpr, Anchor::Top, {0.0f, -28.0f});
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopRight, {-5.0f, -5.0f});

    return true;
}

TimelyLayer* TimelyLayer::create(CreatorLayer* creatorLayer) {
    auto ret = new TimelyLayer();
    if (ret->initAnchored(420.0f, 280.0f, creatorLayer, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    delete(ret);
    return nullptr;
}

void TimelyLayer::onNewDaily(CCObject* sender) {
    m_creatorLayer->onDailyLevel(sender);
}

void TimelyLayer::onNewWeekly(CCObject* sender) {
    m_creatorLayer->onWeeklyLevel(sender);
}

void TimelyLayer::onNewEvent(CCObject* sender) {  
    m_creatorLayer->onEventLevel(sender);
}