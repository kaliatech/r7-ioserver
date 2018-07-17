#pragma once

#include <memory>
#include <string>
#include <vector>

#include "nlohmann-json/json.hpp"
namespace r7 {

class Action
{
public:
    Action();
    Action(const nlohmann::json& jsonObj);
    virtual ~Action() = default;

    void fromJson(const nlohmann::json& jsonObj);

public:
    int time;
    std::string type;
    std::string servoId;
    float targetDeg;
    int duration;
    float accel;
    float deaccel;

};

}
