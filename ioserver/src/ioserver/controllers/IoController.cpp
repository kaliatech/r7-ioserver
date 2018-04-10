#include "IoController.h"

#include "../IoServerContext.h"
#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

IoController::IoController() :
    jsonObj(nullptr) {
}

IoController::IoController(const std::string& type, std::unique_ptr<nlohmann::json> jsonObjParam) :
    type(type),
    jsonObj(std::move(jsonObjParam)) {
    if (jsonObj != nullptr) {
        this->updateFromJson();
    }
}

void IoController::onDbmUpdate(const nlohmann::json& newJsonObj) {
    this->jsonObj.reset(new nlohmann::json(newJsonObj));
    this->updateFromJson();
}

void IoController::updateFromJson() {
    this->id = this->jsonObj->at("id");
    this->type = this->jsonObj->at("controllerTypeId");
    this->ioConnStr = this->jsonObj->at("ioConnStr");

    this->reset();
}
}
