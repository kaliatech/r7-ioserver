#pragma once

#include "DatabaseManager.h"

namespace r7 {


class Repository
{
public:
    Repository(DatabaseManager& dbm);
    virtual ~Repository() = default;

protected:
    DatabaseManager& dbm;
};

}
