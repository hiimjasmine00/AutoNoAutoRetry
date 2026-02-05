#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/loader/Types.hpp>

namespace AutoNoAutoRetry {
    const matjson::Value& getLevelContainer(GJGameLevel* level);
    matjson::Value& getOrCreateLevelContainer(GJGameLevel* level);
    double getPercentage(const matjson::Value& container);
    bool getEnable(const matjson::Value& container);
    bool getNewBest(const matjson::Value& container);
    bool getTestMode(const matjson::Value& container);
}
