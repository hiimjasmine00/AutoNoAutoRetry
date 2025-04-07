#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class ANARSettingsPopup : public geode::Popup<> {
protected:
    int m_value;

    bool setup() override;
public:
    static ANARSettingsPopup* create();

    void onClose(cocos2d::CCObject* sender) override {
        Popup::onClose(sender);
    }
};
