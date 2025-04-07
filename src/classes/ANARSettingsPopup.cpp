#include "ANARSettingsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

ANARSettingsPopup* ANARSettingsPopup::create() {
    auto ret = new ANARSettingsPopup();
    if (ret->initAnchored(220.0f, 150.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ANARSettingsPopup::setup() {
    setID("ANARSettingsPopup");
    setTitle("Auto No Auto-Retry");
    m_title->setID("auto-no-auto-retry-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    auto mod = Mod::get();
    m_value = mod->getSettingValue<int64_t>("percentage");
    auto enabled = mod->getSettingValue<bool>("enable");

    auto percentageInput = TextInput::create(50.0f, "Num");
    percentageInput->setCommonFilter(CommonFilter::Uint);
    percentageInput->setMaxCharCount(3);
    percentageInput->setPosition({ 76.25f, 90.0f });
    percentageInput->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    percentageInput->setString(std::to_string(m_value));
    percentageInput->setEnabled(enabled);
    percentageInput->setAnchorPoint({ 0.0f, 0.5f });
    percentageInput->setID("percentage-input");
    m_mainLayer->addChild(percentageInput);

    auto percentageSlider = Slider::create(nullptr, nullptr, 0.9f);
    percentageSlider->setPosition({ 110.0f, 55.0f });
    percentageSlider->setValue(m_value / 100.0f);
    percentageSlider->m_touchLogic->setEnabled(enabled);
    percentageSlider->setID("percentage-slider");
    m_mainLayer->addChild(percentageSlider);

    auto prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.1f, [this, percentageSlider, percentageInput](auto) {
        m_value = std::max(0, m_value - 1);
        percentageSlider->setValue(m_value / 100.0f);
        percentageInput->setString(std::to_string(m_value));
    });
    prevButton->setPosition({ 60.0f, 90.0f });
    prevButton->setEnabled(enabled);
    prevButton->setID("prev-button");
    m_buttonMenu->addChild(prevButton);

    auto nextButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.1f, [this, percentageSlider, percentageInput](auto) {
        m_value = std::min(100, m_value + 1);
        percentageSlider->setValue(m_value / 100.0f);
        percentageInput->setString(std::to_string(m_value));
    });
    nextButton->setPosition({ 160.0f, 90.0f });
    nextButton->setEnabled(enabled);
    nextButton->setID("next-button");
    m_buttonMenu->addChild(nextButton);

    CCMenuItemExt::assignCallback<SliderThumb>(percentageSlider->m_touchLogic->m_thumb, [this, percentageInput](SliderThumb* sender) {
        m_value = roundf(sender->getValue() * 100.0f);
        percentageInput->setString(std::to_string(m_value));
    });

    percentageInput->setCallback([this, percentageSlider](const std::string& text) {
        m_value = std::clamp(numFromString<int>(text).unwrapOr(0), 0, 100);
        percentageSlider->setValue(m_value / 100.0f);
    });

    auto percentLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    percentLabel->setScale(0.6f);
    percentLabel->setPosition({ 146.25f, 90.0f });
    percentLabel->setAnchorPoint({ 1.0f, 0.5f });
    percentLabel->setID("percentage-label");
    m_mainLayer->addChild(percentLabel);

    auto enabledToggler = CCMenuItemExt::createTogglerWithStandardSprites(0.8f,
        [this, prevButton, nextButton, percentageSlider, percentageInput](CCMenuItemToggler* sender) {
            prevButton->setEnabled(!sender->m_toggled);
            nextButton->setEnabled(!sender->m_toggled);
            percentageSlider->m_touchLogic->setEnabled(!sender->m_toggled);
            percentageInput->setEnabled(!sender->m_toggled);
        });
    enabledToggler->setPosition({ 24.0f, 24.0f });
    enabledToggler->toggle(enabled);
    enabledToggler->setID("enabled-toggle");
    m_buttonMenu->addChild(enabledToggler);

    auto setButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Set", 40, true, "goldFont.fnt", "GJ_button_01.png", 30.0f, 0.8f),
        [this, enabledToggler](auto) {
            auto mod = Mod::get();
            mod->setSettingValue("enable", enabledToggler->m_toggled);
            mod->setSettingValue("percentage", (int64_t)m_value);
            onClose(nullptr);
        });
    setButton->setPosition({ 110.0f, 24.0f });
    setButton->setID("set-button");
    m_buttonMenu->addChild(setButton);

    return true;
}
