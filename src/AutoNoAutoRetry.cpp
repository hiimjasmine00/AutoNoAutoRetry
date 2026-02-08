#include "AutoNoAutoRetry.hpp"
#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Mod.hpp>
#include <jasmine/setting.hpp>

using namespace geode::prelude;

std::pair<matjson::Value*, std::string> getSaveContainer(GJGameLevel* level) {
    std::string id;
    std::string_view saveKey;
    if (level->m_levelType == GJLevelType::Editor) {
        saveKey = "editor";
        id = fmt::to_string(EditorIDs::getID(level));
    }
    else if (level->m_gauntletLevel) {
        saveKey = "gauntlet";
        id = fmt::to_string(level->m_levelID.value());
    }
    else if (auto dailyID = level->m_dailyID.value(); dailyID > 0) {
        saveKey = "periodic";
        id = fmt::to_string(dailyID);
    }
    else {
        saveKey = "normal";
        id = fmt::to_string(level->m_levelID.value());
    }

    return std::make_pair(&Mod::get()->getSaveContainer()[saveKey], std::move(id));
}

const matjson::Value& AutoNoAutoRetry::getLevelContainer(GJGameLevel* level) {
    const auto [saveContainer, id] = getSaveContainer(level);
    return (*saveContainer)[id];
}

matjson::Value& AutoNoAutoRetry::getOrCreateLevelContainer(GJGameLevel* level) {
    auto [saveContainer, id] = getSaveContainer(level);
    return (*saveContainer)[id];
}

template <typename T>
T getProperty(const matjson::Value& container, std::string_view key) {
    return container.get<T>(key).unwrapOr(jasmine::setting::getValue<T>(key));
}

double AutoNoAutoRetry::getPercentage(const matjson::Value& container) {
    return getProperty<double>(container, "percentage");
}

bool AutoNoAutoRetry::getEnable(const matjson::Value& container) {
    return getProperty<bool>(container, "enable");
}

bool AutoNoAutoRetry::getNewBest(const matjson::Value& container) {
    return getProperty<bool>(container, "new-best");
}

bool AutoNoAutoRetry::getTestMode(const matjson::Value& container) {
    return getProperty<bool>(container, "testmode");
}
