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
    this->id = this->jsonObj->at("id").get<std::string>();
    this->type = this->jsonObj->at("controllerTypeId").get<std::string>();
    this->ioConnStr = this->jsonObj->at("ioConnStr").get<std::string>();

    this->reset();
}
}
