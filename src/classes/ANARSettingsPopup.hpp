#include <Geode/ui/Popup.hpp>

class ANARSettingsPopup : public geode::Popup {
protected:
    double m_value;

    bool init(GJGameLevel*);
public:
    static ANARSettingsPopup* create(GJGameLevel*);

    void close();
};
