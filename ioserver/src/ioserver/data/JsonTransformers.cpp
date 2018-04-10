#include "JsonTransformers.h"
#include "nlohmann-json/json.hpp"


#include "../controllers/IoController.h"

#include "../controllers/PololuMaestroController.h"

namespace r7 {
using nlohmann::json;

    void to_json(json& j, const IoController& c) {
        j = json{{"id", c.getId()}, {"controllerTypeId", c.getType()}};
        //alt?
        //j = c.getJsonObj()
    }

//    void from_json(const json& j, IoController& c) {
//        //c.id = j.at("id").get<std::string>();
//        //c.type = j.at("type").get<std::string>();
//    }

    void to_json(json& j, const PololuMaestroController& c) {
        j = json{{"id", c.getId()}, {"controllerTypeId", c.getType()}};
    }

//    void from_json(const json& j, PololuMaestroController& c) {
//        //c.id = j.at("id").get<std::string>();
//        //c.type = j.at("type").get<std::string>();
//    }
}
