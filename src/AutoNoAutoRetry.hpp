#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/loader/Types.hpp>

class AutoNoAutoRetry {
public:
    static matjson::Value getLevelContainer(GJGameLevel* level);
    static matjson::Value& getOrCreateLevelContainer(GJGameLevel* level);
    static double getPercentage(const matjson::Value& container);
    static bool getEnable(const matjson::Value& container);
    static bool getNewBest(const matjson::Value& container);
    static bool getTestMode(const matjson::Value& container);
    static void modify(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks);
};
