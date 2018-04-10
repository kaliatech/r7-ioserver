#pragma once

#include "nlohmann-json/json.hpp"


#include "../controllers/IoController.h"
#include "../controllers/PololuMaestroController.h"

namespace r7 {
    using nlohmann::json;

    void to_json(json& j, const IoController& c);

    //void from_json(const json& j, IoController& c);

    void to_json(json& j, const PololuMaestroController& c);

    //void from_json(const json& j, PololuMaestroController& c);
}
