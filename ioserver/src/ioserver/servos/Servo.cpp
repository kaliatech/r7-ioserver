#include "Servo.h"

#include <memory>
#include "../data/DatabaseManager.h"

namespace r7 {

using std::shared_ptr;

Servo::Servo() :
    jsonObj(nullptr) {
}

Servo::Servo(std::unique_ptr<nlohmann::json> jsonObj) :
    jsonObj(std::move(jsonObj)) {
}

int Servo::getPin() const {
    return this->jsonObj->at("pin");
}

std::string Servo::getControllerId() const {
    return this->jsonObj->at("controllerId");
}


}
