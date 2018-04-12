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
    IoController(const std::string& type, std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~IoController() = default;

    virtual void moveToPulse(const Servo& servo, const int& pulse) = 0;

    virtual const std::string& getId() const {return this->id;};
    virtual const std::string& getIoConnStr() const {return this->ioConnStr;};
    virtual const std::string& getType() const {return this->type;};

    virtual void onDbmUpdate(const nlohmann::json& jsonObj);

    virtual void reset() {};
    virtual void stop() {};

private:
    void updateFromJson();

protected:
    std::string id;
    std::string type; //todo: convert to enum
    std::string ioConnStr;

private:
    std::unique_ptr<nlohmann::json> jsonObj;

};

}
