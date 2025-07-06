#include "../AutoNoAutoRetry.hpp"
#include "ANARSettingsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

ANARSettingsPopup* ANARSettingsPopup::create(GJGameLevel* level) {
    auto ret = new ANARSettingsPopup();
    if (ret->initAnchored(220.0f, 180.0f, level)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ANARSettingsPopup::setup(GJGameLevel* level) {
    setID("ANARSettingsPopup");
    setTitle("Auto No Auto-Retry");
    m_title->setID("auto-no-auto-retry-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    auto container = AutoNoAutoRetry::getLevelContainer(level);
    m_value = AutoNoAutoRetry::getPercentage(container);
    auto enabled = AutoNoAutoRetry::getEnable(container);
    auto newBest = AutoNoAutoRetry::getNewBest(container);
    auto testmode = AutoNoAutoRetry::getTestMode(container);

    auto percentageInput = TextInput::create(80.0f, "Num");
    percentageInput->setFilter(".0123456789");
    percentageInput->setMaxCharCount(6);
    percentageInput->setPosition({ 61.25f, 120.0f });
    percentageInput->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    percentageInput->setString(fmt::format("{:.2f}", m_value));
    percentageInput->setEnabled(enabled);
    percentageInput->setAnchorPoint({ 0.0f, 0.5f });
    percentageInput->setID("percentage-input");
    m_mainLayer->addChild(percentageInput);

    auto percentageSlider = Slider::create(nullptr, nullptr, 0.9f);
    percentageSlider->setPosition({ 110.0f, 85.0f });
    percentageSlider->setValue(m_value / 100.0f);
    percentageSlider->m_touchLogic->setEnabled(enabled);
    percentageSlider->setID("percentage-slider");
    m_mainLayer->addChild(percentageSlider);

    auto prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.1f, [this, percentageSlider, percentageInput](auto) {
        m_value = std::max(0.0, m_value - 1.0);
        percentageSlider->setValue(m_value / 100.0);
        percentageInput->setString(fmt::format("{:.2f}", m_value));
    });
    prevButton->setPosition({ 45.0f, 120.0f });
    prevButton->setEnabled(enabled);
    prevButton->setID("prev-button");
    m_buttonMenu->addChild(prevButton);

    auto nextButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.1f, [this, percentageSlider, percentageInput](auto) {
        m_value = std::min(100.0, m_value + 1.0);
        percentageSlider->setValue(m_value / 100.0);
        percentageInput->setString(fmt::format("{:.2f}", m_value));
    });
    nextButton->setPosition({ 175.0f, 120.0f });
    nextButton->setEnabled(enabled);
    nextButton->setID("next-button");
    m_buttonMenu->addChild(nextButton);

    CCMenuItemExt::assignCallback<SliderThumb>(percentageSlider->m_touchLogic->m_thumb, [this, percentageInput](SliderThumb* sender) {
        m_value = round(sender->getValue() * 10000.0) / 100.0;
        percentageInput->setString(fmt::format("{:.2f}", m_value));
    });

    percentageInput->setCallback([this, percentageSlider](const std::string& text) {
        m_value = std::clamp(numFromString<double>(text).unwrapOr(0.0), 0.0, 100.0);
        percentageSlider->setValue(m_value / 100.0);
    });

    auto percentLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    percentLabel->setScale(0.6f);
    percentLabel->setPosition({ 161.25f, 120.0f });
    percentLabel->setAnchorPoint({ 1.0f, 0.5f });
    percentLabel->setID("percentage-label");
    m_mainLayer->addChild(percentLabel);

    auto newBestToggle = CCMenuItemToggler::createWithStandardSprites(nullptr, nullptr, 0.8f);
    newBestToggle->setPosition({ 40.0f, 60.0f });
    newBestToggle->setEnabled(enabled);
    newBestToggle->toggle(newBest);
    newBestToggle->setID("new-best-toggle");
    m_buttonMenu->addChild(newBestToggle);

    auto newBestLabel = CCLabelBMFont::create("New\nBest", "bigFont.fnt");
    newBestLabel->setScale(0.4f);
    newBestLabel->setPosition({ 60.0f, 60.0f });
    newBestLabel->setAnchorPoint({ 0.0f, 0.5f });
    newBestLabel->setID("new-best-label");
    m_mainLayer->addChild(newBestLabel);

    auto testmodeToggle = CCMenuItemToggler::createWithStandardSprites(nullptr, nullptr, 0.8f);
    testmodeToggle->setPosition({ 140.0f, 60.0f });
    testmodeToggle->setEnabled(enabled);
    testmodeToggle->toggle(testmode);
    testmodeToggle->setID("testmode-toggle");
    m_buttonMenu->addChild(testmodeToggle);

    auto testmodeLabel = CCLabelBMFont::create("Test\nMode", "bigFont.fnt");
    testmodeLabel->setScale(0.4f);
    testmodeLabel->setPosition({ 160.0f, 60.0f });
    testmodeLabel->setAnchorPoint({ 0.0f, 0.5f });
    testmodeLabel->setID("testmode-label");
    m_mainLayer->addChild(testmodeLabel);

    auto enabledToggler = CCMenuItemExt::createTogglerWithStandardSprites(0.8f,
        [this, prevButton, nextButton, percentageSlider, percentageInput, newBestToggle, testmodeToggle](CCMenuItemToggler* sender) {
            prevButton->setEnabled(!sender->m_toggled);
            nextButton->setEnabled(!sender->m_toggled);
            percentageSlider->m_touchLogic->setEnabled(!sender->m_toggled);
            percentageInput->setEnabled(!sender->m_toggled);
            newBestToggle->setEnabled(!sender->m_toggled);
            testmodeToggle->setEnabled(!sender->m_toggled);
        });
    enabledToggler->setPosition({ 24.0f, 24.0f });
    enabledToggler->toggle(enabled);
    enabledToggler->setID("enabled-toggle");
    m_buttonMenu->addChild(enabledToggler);

    auto setButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Set", 0.8f),
        [this, enabledToggler, newBestToggle, testmodeToggle, level](auto) {
            auto& container = AutoNoAutoRetry::getOrCreateLevelContainer(level);
            container["enable"] = enabledToggler->m_toggled;
            container["percentage"] = m_value;
            container["new-best"] = newBestToggle->m_toggled;
            container["testmode"] = testmodeToggle->m_toggled;
            close();
        });
    setButton->setPosition({ 110.0f, 24.0f });
    setButton->setID("set-button");
    m_buttonMenu->addChild(setButton);

    return true;
}

void ANARSettingsPopup::close() {
    onClose(nullptr);
}
