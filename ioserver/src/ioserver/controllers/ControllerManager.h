#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "../data/DatabaseManager.h"
#include "IoController.h"

namespace r7 {

class ControllerManager
{
public:
    ControllerManager(const std::shared_ptr<DatabaseManager> dbm);
    virtual ~ControllerManager() = default;

    std::shared_ptr<IoController> findControllerById(const std::string& controllerId);
public:

private:
    const std::shared_ptr<DatabaseManager> dbm;
    std::unordered_map<std::string, std::shared_ptr<IoController>> controllers;
};

}
