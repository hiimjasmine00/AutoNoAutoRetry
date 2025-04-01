#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class ANARSettingsPopup : public geode::Popup<> {
protected:
    int m_value;
    bool m_enabled;
    geode::TextInput* m_percentageInput;
    CCMenuItemSpriteExtra* m_leftButton;
    CCMenuItemSpriteExtra* m_rightButton;
    Slider* m_slider;

    bool setup() override;
    void onSlider(CCObject* sender);
    void valueChanged(bool updateText);
public:
    static ANARSettingsPopup* create();
};
