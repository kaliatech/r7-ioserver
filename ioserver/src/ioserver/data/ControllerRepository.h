#pragma once

#include <memory>
#include <string>

#include "sqlite3/sqlite3.h"

#include "Repository.h"
#include "../controllers/IoController.h"
#include "../data/DatabaseManager.h"

namespace r7 {

class ControllerRepository : public Repository
{
public:
    ControllerRepository(DatabaseManager& dbm);
    virtual ~ControllerRepository() = default;

    std::unique_ptr<IoController> findById(std::string id);
    void save(IoController& ioController);

};

}
