#pragma once

#include <string>
#include <vector>
#include <memory>

#include "nlohmann-json/json.hpp"
#include "sqlite3/sqlite3.h"

#include <boost/signals2.hpp>

namespace r7 {

class ControllerRepository;
class IoServerConfig;

class DatabaseManager
{
public:
    typedef boost::signals2::signal<void (const std::string& type, const std::string& id, const nlohmann::json& jsonObj)>  signal_dbm_update;

public:
    DatabaseManager(const IoServerConfig& config);
    virtual ~DatabaseManager();

    // virtual void doTest() = 0;
    boost::signals2::connection connect(const signal_dbm_update::slot_type &subscriber);

public:
   sqlite3& getConn() {return *conn;};

   void reinitSchema();

   std::vector<std::string> findAll(const std::string& type);
   std::unique_ptr<nlohmann::json> findById(const std::string& type, const std::string& id);

   void save(const std::string& type, const std::string& prevId, const std::string& newId, const nlohmann::json& obj);

   void deleteObj(const std::string& type, const std::string& id);

private:
    sqlite3* conn;
    signal_dbm_update m_sig;

    ControllerRepository* controllerRepo;
};

}
