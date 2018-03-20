#pragma once

#include "SerialController.h"

#include "nlohmann-json/json.hpp"

namespace r7 {

class PololuMaestroController : public SerialController
{
public:
    PololuMaestroController();
    PololuMaestroController(std::unique_ptr<nlohmann::json> jsonObj);

    virtual ~PololuMaestroController() = default;
};

}
