#pragma once

#include <string>
#include <vector>
#include <memory>

#include "nlohmann-json/json.hpp"
#include "sqlite3/sqlite3.h"

namespace r7 {

class ControllerRepository;

class DatabaseManager
{
public:
    DatabaseManager();
    virtual ~DatabaseManager();

    // virtual void doTest() = 0;

public:
   sqlite3& getConn() {return *conn;};

   void reinitSchema();

   std::vector<std::string> findAll(const std::string& type);
   std::unique_ptr<nlohmann::json> findById(const std::string& type, const std::string& id);

   void save(const std::string& type, const std::string& prevId, const std::string& newId, const nlohmann::json& obj);

   void deleteObj(const std::string& type, const std::string& id);

private:
    sqlite3* conn;

    ControllerRepository* controllerRepo;
};

}
