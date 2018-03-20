#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../data/DatabaseManager.h"
#include "Servo.h"

namespace r7 {

class ServoManager
{
public:
    ServoManager(const std::shared_ptr<DatabaseManager> dbm);
    virtual ~ServoManager() = default;

    std::shared_ptr<Servo> findServoById(const std::string& servoId) const;
public:

private:
    const std::shared_ptr<DatabaseManager> dbm;
};

}
