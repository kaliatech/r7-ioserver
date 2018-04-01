#include "IoController.h"

#include "../IoServerContext.h"
#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

IoController::IoController() :
    jsonObj(nullptr) {
}

IoController::IoController(std::unique_ptr<nlohmann::json> jsonObj) :
    jsonObj(std::move(jsonObj)) {
}

const std::string& IoController::getIoConnStr() {
    //todo:
    this->ioConnStr = this->jsonObj->at("ioConnStr");
    return this->ioConnStr;
}

}
