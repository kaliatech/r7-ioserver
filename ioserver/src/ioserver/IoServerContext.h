#pragma once

#include <string>
#include <boost/thread/mutex.hpp>

#include "data/DatabaseManager.h"
#include "controllers/ControllerManager.h"
#include "servos/ServoManager.h"

namespace r7 {


class IoServerContext
{

private:

public:
    enum Level {DBG, INF, ERR};
    static const char *LevelNames[];
    boost::mutex log_mutex;

public:
    IoServerContext(const std::shared_ptr<DatabaseManager> dbManager, const std::shared_ptr<ControllerManager> controllerManager, const std::shared_ptr<ServoManager> servoManager);
    ~IoServerContext(void);

    void log(const std::string& msg, Level level);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void error(const std::string& msg);

    const std::shared_ptr<DatabaseManager> dbm;
    const std::shared_ptr<ControllerManager> cm;
    const std::shared_ptr<ServoManager> sm;
};
}


