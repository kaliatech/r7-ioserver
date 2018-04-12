#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include <boost/signals2.hpp>

#include "../data/DatabaseManager.h"
#include "IoController.h"

#include "nlohmann-json/json.hpp"


namespace r7 {

//class nlohmann::json;

class ControllerManager
{
public:
    ControllerManager(const std::shared_ptr<DatabaseManager> dbm);
    virtual ~ControllerManager();

    std::shared_ptr<IoController> findControllerById(const std::string& controllerId);
    void onDbmUpdate(const std::string& type, const std::string& id, const nlohmann::json& jsonObj);

    void stop();

public:

private:
    const std::shared_ptr<DatabaseManager> dbm;
    boost::signals2::connection  m_connection;
    std::unordered_map<std::string, std::shared_ptr<IoController>> controllers;
};

}
