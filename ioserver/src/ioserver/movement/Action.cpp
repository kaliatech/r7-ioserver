#include "Action.h"

#include <memory>

namespace r7 {

Action::Action() {
}

Action::Action(const nlohmann::json& jsonObj) {
    this->fromJson(jsonObj);
}

void Action::fromJson(const nlohmann::json& jsonObj) {
    this->time = jsonObj.at("time").get<int>();
    this->type = jsonObj.at("type").get<std::string>();
    this->servoId = jsonObj.at("servoId").get<std::string>();
    this->targetDeg = jsonObj.at("target").get<float>();
    this->duration = jsonObj.at("duration").get<int>();
    this->accel = jsonObj.at("accel").get<float>();
    this->deaccel = jsonObj.at("deaccel").get<float>();
}
}
