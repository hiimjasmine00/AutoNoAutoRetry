#include "../classes/ANARSettingsPopup.hpp"
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/LevelSettingsObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(ANARPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (PlayLayer::get()->m_levelSettings->m_platformerMode) return;

        auto rightButtonMenu = getChildByID("right-button-menu");
        auto anarButtonSprite = CircleButtonSprite::createWithSprite("ANAR_anarBtn_001.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
        anarButtonSprite->getTopNode()->setScale(1.0f);
        anarButtonSprite->setScale(0.6f);
        auto anarButton = CCMenuItemSpriteExtra::create(anarButtonSprite, this, menu_selector(ANARPauseLayer::onAutoNoAutoRetry));
        anarButton->setID("settings-button"_spr);
        rightButtonMenu->addChild(anarButton);
        rightButtonMenu->updateLayout();
    }

    void onAutoNoAutoRetry(CCObject* sender) {
        ANARSettingsPopup::create()->show();
    }
};
