#include <Geode/ui/Popup.hpp>

class ANARSettingsPopup : public geode::Popup<GJGameLevel*> {
protected:
    double m_value;

    bool setup(GJGameLevel*) override;
public:
    static ANARSettingsPopup* create(GJGameLevel*);

    void close();
};
