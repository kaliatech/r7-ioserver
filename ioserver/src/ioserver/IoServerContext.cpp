#include "IoServerContext.h"
#include <iostream>
#include <boost/thread.hpp>

namespace r7 {

const char* IoServerContext::LevelNames[] = { "DEBUG", "INFO", "ERROR" };

IoServerContext::IoServerContext(const std::shared_ptr<DatabaseManager> dbManager,
                                 const std::shared_ptr<ControllerManager> controllerManager,
                                 const std::shared_ptr<ServoManager> servoManager) :
    dbm(dbManager),
    cm(controllerManager),
    sm(servoManager)
{
}

IoServerContext::~IoServerContext(void)
{
}

void IoServerContext::log(const std::string &msg, Level level)
{
    this->log_mutex.lock();
    std::cout << LevelNames[level] << ":" << msg << std::endl;
    this->log_mutex.unlock();
}

void IoServerContext::debug(const std::string &msg)
{
   this->log(msg, IoServerContext::DBG);
}

void IoServerContext::info(const std::string &msg)
{
    this->log(msg, IoServerContext::INF);
}

void IoServerContext::error(const std::string &msg)
{
    this->log(msg, IoServerContext::ERR);
}

}

