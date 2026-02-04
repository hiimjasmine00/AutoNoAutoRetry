#include "../AutoNoAutoRetry.hpp"
#include "../classes/ANARSettingsPopup.hpp"
#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <jasmine/hook.hpp>

using namespace geode::prelude;

class $modify(ANARPlayLayer, PlayLayer) {
    static void onModify(ModifyBase<ModifyDerive<ANARPlayLayer, PlayLayer>>& self) {
        jasmine::hook::modify(self.m_hooks, "enable");
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        if (auto popup = CCScene::get()->getChildByType<ANARSettingsPopup>(0)) popup->close();
    }

    float getPercentage() {
        auto level = m_level;
        auto percent = level->m_normalPercent.value();
        if (percent > 99) return 100.0f;

        auto mod = Loader::get()->getLoadedMod("raydeeux_thesillydoggo.decimalpercentages");
        if (!mod) return percent;

        auto id = level->m_levelID.value();
        std::string str;
        if (level->m_levelType == GJLevelType::Editor) {
            str = fmt::format("percentage_normal_local_{}", EditorIDs::getID(level));
        }
        else if (level->m_gauntletLevel) {
            str = fmt::format("percentage_normal_gauntlet_{}", id);
        }
        else if (auto dailyID = level->m_dailyID.value(); dailyID > 0) {
            str = fmt::format("percentage_normal_{}_periodic_{}", id, dailyID);
        }
        else {
            str = fmt::format("percentage_normal_{}", id);
        }

        return mod->hasSavedValue(str) ? mod->getSavedValue<float>(str) : percent;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        auto& container = AutoNoAutoRetry::getLevelContainer(m_level);

        if (
            object == m_anticheatSpike || m_isPlatformer || m_isPracticeMode ||
            !AutoNoAutoRetry::getEnable(container) || (!AutoNoAutoRetry::getTestMode(container) && m_isTestMode)
        ) return PlayLayer::destroyPlayer(player, object);

        auto GM = GameManager::get();
        if (!GM->getGameVariable(GameVar::AutoRetry)) return PlayLayer::destroyPlayer(player, object);

        auto percent = getCurrentPercent();
        if (percent < AutoNoAutoRetry::getPercentage(container) && (!AutoNoAutoRetry::getNewBest(container) || percent <= getPercentage())) {
            return PlayLayer::destroyPlayer(player, object);
        }

        GM->setGameVariable(GameVar::AutoRetry, false);
        PlayLayer::destroyPlayer(player, object);
        GM->setGameVariable(GameVar::AutoRetry, true);
    }
};
