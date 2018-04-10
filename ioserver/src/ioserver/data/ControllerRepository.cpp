#include "ControllerRepository.h"

#include "nlohmann-json/json.hpp"

#include "DatabaseManager.h"
#include "../controllers/IoController.h"
#include "../controllers/PololuMaestroController.h"
#include "ioserver/IoServerException.h"

#include "sqlite3/sqlite3.h"

#include "JsonTransformers.h"

namespace r7 {

using json = nlohmann::json;

ControllerRepository::ControllerRepository(DatabaseManager& dbm)  :
    Repository(dbm)
{
}

std::unique_ptr<IoController> ControllerRepository::findById(std::string id) {
    return 0;
}

void ControllerRepository::save(IoController& ioController) {

    //PololuMaestroController ioController2;
    json js = ioController;

    // json js;

    sqlite3* db = &this->dbm.getConn();
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "REPLACE INTO Controllers (id, type, obj) VALUES (?, ?, ?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw IoServerException(sqlite3_errmsg(db));

    rc = sqlite3_bind_text(stmt, 1, ioController.getId().c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    rc = sqlite3_bind_text(stmt, 2, ioController.getType().c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    rc = sqlite3_bind_text(stmt, 3, js.dump().c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    //      if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
    //          string errmsg(sqlite3_errmsg(db));
    //          sqlite3_finalize(stmt);
    //          throw errmsg;
    //      }

    sqlite3_finalize(stmt);
}

}
