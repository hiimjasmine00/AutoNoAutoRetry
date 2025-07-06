#include <Geode/GeneratedPredeclare.hpp>
#include <matjson.hpp>

#define ANAR_MODIFY \
    static void onModify(auto& self) { \
        auto mod = Mod::get(); \
        auto enabled = mod->getSettingValue<bool>("enable"); \
        auto& hooks = self.m_hooks; \
        for (auto& [name, hook] : hooks) { \
            hook->setAutoEnable(enabled); \
        } \
        if (!hooks.empty()) listenForSettingChangesV3<bool>("enable", [hooks](bool value) { \
            for (auto& [name, hook] : hooks) { \
                (void)(value ? hook->enable().inspectErr([&name](const std::string& err) { \
                    log::error("Failed to enable {} hook: {}", name, err); \
                }) : hook->disable().inspectErr([&name](const std::string& err) { \
                    log::error("Failed to disable {} hook: {}", name, err); \
                })); \
            } \
        }, mod); \
    } \

class AutoNoAutoRetry {
public:
    static matjson::Value getLevelContainer(GJGameLevel* level);
    static matjson::Value& getOrCreateLevelContainer(GJGameLevel* level);
    static double getPercentage(const matjson::Value& container);
    static bool getEnable(const matjson::Value& container);
    static bool getNewBest(const matjson::Value& container);
    static bool getTestMode(const matjson::Value& container);
};
