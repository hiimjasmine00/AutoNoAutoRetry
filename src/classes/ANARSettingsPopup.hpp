#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class ANARSettingsPopup : public geode::Popup, public TextInputDelegate {
protected:
    double m_value;
    GJGameLevel* m_level;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    geode::TextInput* m_percentageInput;
    Slider* m_percentageSlider;
    CCMenuItemToggler* m_enabledToggler;
    CCMenuItemToggler* m_newBestToggle;
    CCMenuItemToggler* m_testmodeToggle;

    bool init(GJGameLevel*);
    void onLeft(cocos2d::CCObject*);
    void onRight(cocos2d::CCObject*);
    void sliderChanged(cocos2d::CCObject*);
    void textChanged(CCTextInputNode*) override;
    void onEnable(cocos2d::CCObject*);
    void onSet(cocos2d::CCObject*);
public:
    static ANARSettingsPopup* create(GJGameLevel*);

    void close();
};
