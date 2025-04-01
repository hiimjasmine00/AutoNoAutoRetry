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
    setTitle("Auto No Auto-Retry");
    m_noElasticity = true;
    auto mod = Mod::get();
    m_value = mod->getSettingValue<int64_t>("percentage");
    m_enabled = mod->getSettingValue<bool>("enable");

    m_percentageInput = TextInput::create(50.0f, "Num");
    m_percentageInput->setCommonFilter(CommonFilter::Uint);
    m_percentageInput->setPosition({ 76.25f, 90.0f });
    m_percentageInput->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    m_percentageInput->setString(std::to_string(m_value));
    m_percentageInput->setCallback([this](std::string const& text) {
        auto value = text.empty() ? 0 : numFromString<int>(text).unwrapOr(0);
        m_value = std::clamp(value, 0, 100);
        valueChanged(value != m_value);
    });
    m_percentageInput->setEnabled(m_enabled);
    m_percentageInput->setAnchorPoint({ 0.0f, 0.5f });
    m_mainLayer->addChild(m_percentageInput);

    m_leftButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.1f, [this](auto) {
        m_value = std::max(0, m_value - 1);
        valueChanged(true);
    });
    m_leftButton->setPosition({ 60.0f, 90.0f });
    m_leftButton->setEnabled(m_enabled);
    m_buttonMenu->addChild(m_leftButton);

    m_rightButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.1f, [this](auto) {
        m_value = std::min(100, m_value + 1);
        valueChanged(true);
    });
    m_rightButton->setPosition({ 160.0f, 90.0f });
    m_rightButton->setEnabled(m_enabled);
    m_buttonMenu->addChild(m_rightButton);

    m_slider = Slider::create(this, menu_selector(ANARSettingsPopup::onSlider), 0.9f);
    m_slider->setPosition({ 110.0f, 55.0f });
    m_slider->setValue(m_value / 100.0f);
    m_slider->m_touchLogic->setEnabled(m_enabled);
    m_mainLayer->addChild(m_slider);

    auto percentLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    percentLabel->setScale(0.6f);
    percentLabel->setPosition({ 146.25f, 90.0f });
    percentLabel->setAnchorPoint({ 1.0f, 0.5f });
    m_mainLayer->addChild(percentLabel);

    auto enabledToggler = CCMenuItemExt::createTogglerWithStandardSprites(0.8f, [this](auto) {
        m_enabled = !m_enabled;
        m_leftButton->setEnabled(m_enabled);
        m_rightButton->setEnabled(m_enabled);
        m_slider->m_touchLogic->setEnabled(m_enabled);
        m_percentageInput->setEnabled(m_enabled);
    });
    enabledToggler->setPosition({ 24.0f, 24.0f });
    enabledToggler->toggle(m_enabled);
    m_buttonMenu->addChild(enabledToggler);

    auto setButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Set", 40, true, "goldFont.fnt", "GJ_button_01.png", 30.0f, 0.8f), [this](auto) {
        auto mod = Mod::get();
        mod->setSettingValue("enable", m_enabled);
        mod->setSettingValue("percentage", (int64_t)m_value);
        onClose(nullptr);
    });
    setButton->setPosition({ 110.0f, 24.0f });
    m_buttonMenu->addChild(setButton);

    return true;
}

void ANARSettingsPopup::onSlider(CCObject* sender) {
    m_value = std::round(static_cast<SliderThumb*>(sender)->getValue() * 100.0f);
    valueChanged(true);
}

void ANARSettingsPopup::valueChanged(bool updateText) {
    m_slider->setValue(m_value / 100.0f);
    if (updateText) m_percentageInput->setString(std::to_string(m_value));
}
