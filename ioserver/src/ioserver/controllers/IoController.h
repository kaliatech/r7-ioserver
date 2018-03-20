#pragma once

#include <boost/shared_ptr.hpp>
#include <string>

class AsyncSerialEvent;

#include "nlohmann-json/json.hpp"

namespace r7 {

class Servo;

class IoController
{
public:
    IoController();
    IoController(std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~IoController() = default;

    virtual void moveToPulse(const Servo& servo, const int& pulse) = 0;


public:
    std::string id;

private:
    std::unique_ptr<nlohmann::json> jsonObj;

};

}
