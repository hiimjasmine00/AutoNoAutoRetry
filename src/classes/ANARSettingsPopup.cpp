#include "../AutoNoAutoRetry.hpp"
#include "ANARSettingsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

ANARSettingsPopup* ANARSettingsPopup::create(GJGameLevel* level) {
    auto ret = new ANARSettingsPopup();
    if (ret->init(level)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ANARSettingsPopup::init(GJGameLevel* level) {
    if (!Popup::init(220.0f, 180.0f)) return false;

    setID("ANARSettingsPopup");
    setTitle("Auto No Auto-Retry");
    m_title->setID("auto-no-auto-retry-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    auto& container = AutoNoAutoRetry::getLevelContainer(level);
    m_value = AutoNoAutoRetry::getPercentage(container);
    auto enabled = AutoNoAutoRetry::getEnable(container);
    auto newBest = AutoNoAutoRetry::getNewBest(container);
    auto testmode = AutoNoAutoRetry::getTestMode(container);

    m_percentageInput = TextInput::create(80.0f, "Num");
    m_percentageInput->setFilter(".0123456789");
    m_percentageInput->setMaxCharCount(6);
    m_percentageInput->setPosition({ 61.25f, 120.0f });
    m_percentageInput->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    m_percentageInput->setString(fmt::format("{:.2f}", m_value));
    m_percentageInput->setEnabled(enabled);
    m_percentageInput->setAnchorPoint({ 0.0f, 0.5f });
    m_percentageInput->setDelegate(this);
    m_percentageInput->setID("percentage-input");
    m_mainLayer->addChild(m_percentageInput);

    m_percentageSlider = Slider::create(this, menu_selector(ANARSettingsPopup::sliderChanged), 0.9f);
    m_percentageSlider->setPosition({ 110.0f, 85.0f });
    m_percentageSlider->setValue(m_value / 100.0f);
    m_percentageSlider->m_touchLogic->setEnabled(enabled);
    m_percentageSlider->setID("percentage-slider");
    m_mainLayer->addChild(m_percentageSlider);

    auto prevSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    prevSprite->setScale(1.1f);
    m_prevButton = CCMenuItemSpriteExtra::create(prevSprite, this, menu_selector(ANARSettingsPopup::onLeft));
    m_prevButton->setPosition({ 45.0f, 120.0f });
    m_prevButton->setEnabled(enabled);
    m_prevButton->setID("prev-button");
    m_buttonMenu->addChild(m_prevButton);

    auto nextSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    nextSprite->setScale(1.1f);
    m_nextButton = CCMenuItemSpriteExtra::create(nextSprite, this, menu_selector(ANARSettingsPopup::onRight));
    m_nextButton->setPosition({ 175.0f, 120.0f });
    m_nextButton->setEnabled(enabled);
    m_nextButton->setID("next-button");
    m_buttonMenu->addChild(m_nextButton);

    auto percentLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    percentLabel->setScale(0.6f);
    percentLabel->setPosition({ 161.25f, 120.0f });
    percentLabel->setAnchorPoint({ 1.0f, 0.5f });
    percentLabel->setID("percentage-label");
    m_mainLayer->addChild(percentLabel);

    auto newBestOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    newBestOffSprite->setScale(0.8f);
    auto newBestOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    newBestOnSprite->setScale(0.8f);
    m_newBestToggle = CCMenuItemToggler::create(newBestOffSprite, newBestOnSprite, nullptr, nullptr);
    m_newBestToggle->setPosition({ 40.0f, 60.0f });
    m_newBestToggle->setEnabled(enabled);
    m_newBestToggle->toggle(newBest);
    m_newBestToggle->setID("new-best-toggle");
    m_buttonMenu->addChild(m_newBestToggle);

    auto newBestLabel = CCLabelBMFont::create("New\nBest", "bigFont.fnt");
    newBestLabel->setScale(0.4f);
    newBestLabel->setPosition({ 60.0f, 60.0f });
    newBestLabel->setAnchorPoint({ 0.0f, 0.5f });
    newBestLabel->setID("new-best-label");
    m_mainLayer->addChild(newBestLabel);

    auto testmodeOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    testmodeOffSprite->setScale(0.8f);
    auto testmodeOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    testmodeOnSprite->setScale(0.8f);
    m_testmodeToggle = CCMenuItemToggler::create(testmodeOffSprite, testmodeOnSprite, nullptr, nullptr);
    m_testmodeToggle->setPosition({ 140.0f, 60.0f });
    m_testmodeToggle->setEnabled(enabled);
    m_testmodeToggle->toggle(testmode);
    m_testmodeToggle->setID("testmode-toggle");
    m_buttonMenu->addChild(m_testmodeToggle);

    auto testmodeLabel = CCLabelBMFont::create("Test\nMode", "bigFont.fnt");
    testmodeLabel->setScale(0.4f);
    testmodeLabel->setPosition({ 160.0f, 60.0f });
    testmodeLabel->setAnchorPoint({ 0.0f, 0.5f });
    testmodeLabel->setID("testmode-label");
    m_mainLayer->addChild(testmodeLabel);

    auto enabledOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    enabledOffSprite->setScale(0.8f);
    auto enabledOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    enabledOnSprite->setScale(0.8f);
    m_enabledToggler = CCMenuItemToggler::create(enabledOffSprite, enabledOnSprite, this, menu_selector(ANARSettingsPopup::onEnable));
    m_enabledToggler->setPosition({ 24.0f, 24.0f });
    m_enabledToggler->toggle(enabled);
    m_enabledToggler->setID("enabled-toggle");
    m_buttonMenu->addChild(m_enabledToggler);

    auto setButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Set", 0.8f), this, menu_selector(ANARSettingsPopup::onSet));
    setButton->setPosition({ 110.0f, 24.0f });
    setButton->setID("set-button");
    m_buttonMenu->addChild(setButton);

    return true;
}

void ANARSettingsPopup::onLeft(CCObject* sender) {
    m_value = std::max(0.0, m_value - 1.0);
    m_percentageSlider->setValue(m_value / 100.0);
    m_percentageInput->setString(fmt::format("{:.2f}", m_value));
}

void ANARSettingsPopup::onRight(CCObject* sender) {
    m_value = std::min(100.0, m_value + 1.0);
    m_percentageSlider->setValue(m_value / 100.0);
    m_percentageInput->setString(fmt::format("{:.2f}", m_value));
}

void ANARSettingsPopup::sliderChanged(CCObject* sender) {
    m_value = round(static_cast<SliderThumb*>(sender)->getValue() * 10000.0) / 100.0;
    m_percentageInput->setString(fmt::format("{:.2f}", m_value));
}

void ANARSettingsPopup::textChanged(CCTextInputNode* input) {
    if (auto num = numFromString<double>(input->getString())) {
        m_value = num.unwrap();
    }
    m_value = std::clamp(m_value, 0.0, 100.0);
    m_percentageSlider->setValue(m_value / 100.0);
}

void ANARSettingsPopup::onEnable(CCObject* sender) {
    auto enabled = !static_cast<CCMenuItemToggler*>(sender)->m_toggled;
    m_prevButton->setEnabled(enabled);
    m_nextButton->setEnabled(enabled);
    m_percentageSlider->m_touchLogic->setEnabled(enabled);
    m_percentageInput->setEnabled(enabled);
    m_newBestToggle->setEnabled(enabled);
    m_testmodeToggle->setEnabled(enabled);
}

void ANARSettingsPopup::onSet(CCObject* sender) {
    auto& container = AutoNoAutoRetry::getOrCreateLevelContainer(m_level);
    container["enable"] = m_enabledToggler->m_toggled;
    container["percentage"] = m_value;
    container["new-best"] = m_newBestToggle->m_toggled;
    container["testmode"] = m_testmodeToggle->m_toggled;
    close();
}

void ANARSettingsPopup::close() {
    onClose(nullptr);
}
