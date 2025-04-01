#include "../classes/ANARSettingsPopup.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ANARPlayLayer, PlayLayer) {
    static void onModify(ModifyBase<ModifyDerive<ANARPlayLayer, PlayLayer>>& self) {
        (void)self.getHook("PlayLayer::destroyPlayer").map([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable"));

            listenForSettingChangesV3<bool>("enable", [hook](bool value) {
                (void)(value ? hook->enable().mapErr([](const std::string& err) {
                    return log::error("Failed to enable PlayLayer::destroyPlayer hook: {}", err), err;
                }) : hook->disable()).mapErr([](const std::string& err) {
                    return log::error("Failed to disable PlayLayer::destroyPlayer hook: {}", err), err;
                });
            }, mod);

            return hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get PlayLayer::destroyPlayer hook: {}", err), err;
        });
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        if (auto popup = CCScene::get()->getChildByType<ANARSettingsPopup>(0)) {
            popup->setKeypadEnabled(false);
            popup->setTouchEnabled(false);
            popup->removeFromParentAndCleanup(true);
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        auto GM = GameManager::get();
        auto autoRetryEnabled = GM->getGameVariable("0026");
        if (!autoRetryEnabled) {
            PlayLayer::destroyPlayer(player, object);
            return;
        }

        if (!m_isPracticeMode && !player->m_isPlatformer && getCurrentPercentInt() >= Mod::get()->getSettingValue<int64_t>("percentage"))
            GM->setGameVariable("0026", false);

        PlayLayer::destroyPlayer(player, object);

        GM->setGameVariable("0026", autoRetryEnabled);
    }
};
