#include "ioserver/IoServerContext.h"
#include <iostream>
#include <boost/thread.hpp>

namespace r7 {

const char* IoServerContext::LevelNames[] = { "DEBUG", "INFO", "ERROR" };

boost::mutex IoServerContext::log_mutex;

IoServerContext::IoServerContext(void)
{
}


IoServerContext::~IoServerContext(void)
{
}

void IoServerContext::init(void)
{
}

void IoServerContext::log(const std::string &msg, Level level)
{
    IoServerContext::log_mutex.lock();
    std::cout << LevelNames[level] << ":" << msg << std::endl;
    IoServerContext::log_mutex.unlock();
}

void IoServerContext::debug(const std::string &msg)
{
    IoServerContext::log(msg, IoServerContext::DBG);
}

void IoServerContext::info(const std::string &msg)
{
    IoServerContext::log(msg, IoServerContext::INF);
}

void IoServerContext::error(const std::string &msg)
{
    IoServerContext::log(msg, IoServerContext::ERR);
}

}

