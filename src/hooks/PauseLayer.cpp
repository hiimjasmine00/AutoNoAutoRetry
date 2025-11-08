#include "../AutoNoAutoRetry.hpp"
#include "../classes/ANARSettingsPopup.hpp"
#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <jasmine/hook.hpp>

using namespace geode::prelude;

class $modify(ANARPauseLayer, PauseLayer) {
    static void onModify(ModifyBase<ModifyDerive<ANARPauseLayer, PauseLayer>>& self) {
        jasmine::hook::modify(self.m_hooks, "enable");
    }

    void customSetup() {
        PauseLayer::customSetup();

        if (GJBaseGameLayer::get()->m_isPlatformer) return;

        auto rightButtonMenu = getChildByID("right-button-menu");
        auto anarSprite = CircleButtonSprite::createWithSprite("ANAR_anarBtn_001.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
        anarSprite->getTopNode()->setScale(1.0f);
        anarSprite->setScale(0.6f);
        auto anarButton = CCMenuItemSpriteExtra::create(anarSprite, this, menu_selector(ANARPauseLayer::onAutoNoAutoRetry));
        anarButton->setID("settings-button"_spr);
        rightButtonMenu->addChild(anarButton);
        rightButtonMenu->updateLayout();
    }

    void onAutoNoAutoRetry(CCObject* sender) {
        ANARSettingsPopup::create(GJBaseGameLayer::get()->m_level)->show();
    }
};
