#pragma once

#include <memory>
#include <string>

#include "nlohmann-json/json.hpp"
namespace r7 {

class Servo
{
public:
    Servo();
    Servo(std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~Servo() = default;

public:
    int getPin() const;
    std::string getControllerId() const;

private:
    std::unique_ptr<nlohmann::json> jsonObj;
};

}
