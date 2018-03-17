#pragma once

#include <string>
#include <boost/thread/mutex.hpp>

//class DatabaseManager;
#include "data/DatabaseManager.h"

namespace r7 {


class IoServerContext
{

private:

public:
    enum Level {DBG, INF, ERR};
    static const char *LevelNames[];
    boost::mutex log_mutex;

public:
    IoServerContext(std::shared_ptr<DatabaseManager> dbManager);
    ~IoServerContext(void);

    void log(const std::string& msg, Level level);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void error(const std::string& msg);

    const std::shared_ptr<DatabaseManager> dbm;
};
}


