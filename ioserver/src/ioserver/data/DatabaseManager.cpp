#include "DatabaseManager.h"

#include <fstream>
#include <memory>

#include "sqlite3/sqlite3.h"
#include "ioserver/IoServerException.h"
#include "JsonTransformers.h"

#include "ControllerRepository.h"

namespace r7 {

DatabaseManager::DatabaseManager()
{
    if (sqlite3_open("r7-ioserver.db", &this->conn) != SQLITE_OK )
    {
        throw IoServerException("Unable to open db");
    }

    this->controllerRepo = new ControllerRepository(*this);
}

void DatabaseManager::reinitSchema()
{
    std::ifstream iFile("data/db-init.sql");
    bool fileExists = (bool)iFile;
    if (!fileExists) {
        throw IoServerException("/data/db-init.sql file not found");
    }

    std::stringstream sqlBuffer;
    sqlBuffer << iFile.rdbuf();
    std::string sql = sqlBuffer.str();

    char* errMsg;


    sqlite3* db = &this->getConn();
    int rc =  sqlite3_exec(
                db,
                sql.c_str(),                           /* SQL to be evaluated */
                NULL, // int (*callback)(void*,int,char**,char**)         /* Callback function */
                NULL, // void *,        /* 1st argument to callback */
                &errMsg  // char **errmsg  /* Error msg written here */
                );


    if (rc != SQLITE_OK) {
        std::string msg = std::string("Error executing DB init script. Msg: ") + errMsg;
        sqlite3_free(errMsg);
        throw IoServerException(msg);
    }
}


std::vector<std::string> DatabaseManager::findAll(const std::string& type) {
    sqlite3* db = &this->getConn();
    sqlite3_stmt *stmt;

    std::ostringstream sql;
    sql << "SELECT id, obj FROM ";
    sql << type;
    sql << " ORDER BY id";
    auto sqlStr = sql.str();

    int rc = sqlite3_prepare_v2(db, sqlStr.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw IoServerException(sqlite3_errmsg(db));

    std::vector<std::string> objs;

    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW) {
        const unsigned char* objTxt  = sqlite3_column_text (stmt, 1);
        int objTxtBytes = sqlite3_column_bytes(stmt, 1);
        std::string jsonStr(reinterpret_cast<const char*>(objTxt), objTxtBytes);
        objs.push_back(jsonStr);
        rc = sqlite3_step(stmt);
    }
    if (rc != SQLITE_DONE) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    sqlite3_finalize(stmt);
    return objs;
}

std::unique_ptr<nlohmann::json> DatabaseManager::findById(const std::string& type, const std::string& id) {
    sqlite3* db = &this->getConn();
    sqlite3_stmt *stmt;

    std::ostringstream sql;
    sql << "SELECT id, obj FROM ";
    sql << type;
    sql << " WHERE id=?";
    auto sqlStr = sql.str();

    int rc = sqlite3_prepare_v2(db, sqlStr.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw IoServerException(sqlite3_errmsg(db));

    rc = sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::make_unique<nlohmann::json>(nullptr);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    //bytes = sqlite3_column_bytes (stmt, 0);
    const unsigned char* objTxt  = sqlite3_column_text (stmt, 1);
    std::string jsonStr(reinterpret_cast<const char*>(objTxt));
    nlohmann::json jsonObj = nlohmann::json::parse(jsonStr);

    sqlite3_finalize(stmt);

    //TODO: Seems inefficient to use copy constructor. How to parse in constructor?
    return std::make_unique<nlohmann::json>(jsonObj);
}

void DatabaseManager::save(const std::string& type, const std::string& prevId, const std::string& newId, const nlohmann::json& obj) {
    sqlite3* db = &this->getConn();
    sqlite3_stmt *stmt;

    std::ostringstream sql;
//    sql << "REPLACE INTO ";
//    sql << type;
//    sql << " (id, obj) VALUES (?, ?)";

    bool updating = false;
    auto existingJsonObj = this->findById(type, prevId);
    if (!existingJsonObj->empty()) {
        updating = true;
        sql << "UPDATE ";
        sql << type;
        sql << " SET id=?, obj=?";
        sql << " WHERE id=?";
    }
    else {
        sql << "INSERT INTO ";
        sql << type;
        sql << " (id, obj) VALUES (?, ?)";
    }

    auto sqlStr = sql.str();

    int rc = sqlite3_prepare_v2(db, sqlStr.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw IoServerException(sqlite3_errmsg(db));

    rc = sqlite3_bind_text(stmt, 1, newId.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    std::string objStr = obj.dump();
    const char* str =objStr.c_str();
    rc = sqlite3_bind_text(stmt, 2, str, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    if (updating) {
        rc = sqlite3_bind_text(stmt, 3, prevId.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            IoServerException e(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw e;
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    sqlite3_finalize(stmt);

};

void DatabaseManager::deleteObj(const std::string& type, const std::string& id) {
    sqlite3* db = &this->getConn();
    sqlite3_stmt *stmt;

    std::ostringstream sql;
    sql << "DELETE FROM ";
    sql << type;
    sql << " WHERE id=?";

    auto sqlStr = sql.str();

    int rc = sqlite3_prepare_v2(db, sqlStr.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw IoServerException(sqlite3_errmsg(db));

    rc = sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        IoServerException e(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw e;
    }

    sqlite3_finalize(stmt);
};

DatabaseManager::~DatabaseManager() {
    sqlite3_close(this->conn);
};

//sqlite3_stmt *stmt;
// int rc = sqlite3_prepare_v2(db, "SELECT FirstName, LastName, Age"
//                                 " FROM customerTable"
//                                 " WHERE Id = ?", -1, &stmt, NULL);
// if (rc != SQLITE_OK)
//     throw string(sqlite3_errmsg(db));

// rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
// if (rc != SQLITE_OK) {                 // really necessary, but recommended
//     string errmsg(sqlite3_errmsg(db)); // (especially for strings) to avoid
//     sqlite3_finalize(stmt);            // formatting problems and SQL
//     throw errmsg;                      // injection attacks.
// }

// rc = sqlite3_step(stmt);
// if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
//     string errmsg(sqlite3_errmsg(db));
//     sqlite3_finalize(stmt);
//     throw errmsg;
// }
// if (rc == SQLITE_DONE) {
//     sqlite3_finalize(stmt);
//     throw string("customer not found");
// }

// this->id         = id;
// this->first_name = string(sqlite3_column_text(stmt, 0));
// this->last_name  = string(sqlite3_column_text(stmt, 1));
// this->age        =        sqlite3_column_int(stmt, 2);

// sqlite3_finalize(stmt);

}
