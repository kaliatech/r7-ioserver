#include "ControllerManager.h"

#include <memory>
#include "../data/DatabaseManager.h"
#include "PololuMaestroController.h"

#include "nlohmann-json/json.hpp"

namespace r7 {

using std::shared_ptr;

ControllerManager::ControllerManager(const shared_ptr<DatabaseManager> dbm) :
    dbm(dbm),
    controllers()
{
    //TODO: Set self up as listener to dbm.
    //Keep controllers and servos in memory, refreshing when dbm signals a change
}

std::shared_ptr<IoController> ControllerManager::findControllerById(const std::string& controllerId) {

    //TODO: eventually we should cache. For now, look up every time:
    auto pair = this->controllers.find(controllerId);
    if (pair == this->controllers.end()) {
        std::unique_ptr<nlohmann::json> jsonObj = this->dbm->findById("controllers", controllerId);
        if (jsonObj == nullptr) {
            return nullptr;
        }
        std::shared_ptr<IoController> controller = std::make_shared<PololuMaestroController>(std::move(jsonObj));

        controllers.emplace(controllerId, controller);
        return controller;
    }
    else {
        return pair->second;
    }
}

}
