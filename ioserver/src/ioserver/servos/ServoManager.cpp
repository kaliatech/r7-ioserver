#include "ServoManager.h"

#include <memory>
#include "../data/DatabaseManager.h"

#include "nlohmann-json/json.hpp"

namespace r7 {

using std::shared_ptr;

ServoManager::ServoManager(const shared_ptr<DatabaseManager> dbm) :
    dbm(dbm)
{
    //TODO: Set self up as listener to dbm.
    //Keep controllers and servos in memory, refreshing when dbm signals a change
}

std::shared_ptr<Servo> ServoManager::findServoById(const std::string& servoId) const{
    //TODO: eventually we should cache. For now, look up every time:
    std::unique_ptr<nlohmann::json> jsonObj = this->dbm->findById("servos", servoId);
    if (jsonObj == nullptr) {
        return nullptr;
    }

//    Servo servoStack(std::move(jsonObj));

//    std::unique_ptr<nlohmann::json> jsonPtr = std::move(jsonObj);
//    Servo servo2(std::move(jsonPtr));

     std::shared_ptr<Servo> servo = std::make_shared<Servo>(std::move(jsonObj));
     return servo;
}

//onDataChange(string type, string id, shared_ptr<json> oldObj, shared_ptr<json> newObj) {
//}

}
