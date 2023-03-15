#include "ControllerManager.h"

#include <memory>
#include "../data/DatabaseManager.h"
#include "PololuMaestroController.h"

#include "nlohmann-json/json.hpp"
#include "easyloggingpp/easylogging++.h"

namespace r7 {

using std::shared_ptr;

ControllerManager::ControllerManager(const shared_ptr<DatabaseManager> dbm) :
    dbm(dbm),
    controllers()
{
    this->m_connection = dbm->connect(boost::bind(&ControllerManager::onDbmUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

ControllerManager::~ControllerManager() {
    this->m_connection.disconnect();
}

void ControllerManager::onDbmUpdate(const std::string& type, const std::string& id, const nlohmann::json& jsonObj) {
     // LOG(DEBUG) << "onDbmUpdate: " << type;

     auto pair = this->controllers.find(id);

     // If not in memory, ignore
     if (pair == this->controllers.end()) {
         return;
     }

     std::shared_ptr<IoController> controller = pair->second;
     if (controller->getId() != id) {
         // Clear from cache and reinsert with new id
         controllers.erase(controller->getId());
         controllers.emplace(id, controller);
     }

     controller->onDbmUpdate(jsonObj);
}

std::shared_ptr<IoController> ControllerManager::findControllerById(const std::string& controllerId) {

    //TODO: eventually we should cache. For now, look up every time:
    auto pair = this->controllers.find(controllerId);

    // If equals end, then not found
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

void ControllerManager::stop() {
    std::unordered_map<std::string, std::shared_ptr<IoController>>::iterator it = controllers.begin();
    while(it != controllers.end())
    {
        it->second->stop();
        it++;
    }
}
}
