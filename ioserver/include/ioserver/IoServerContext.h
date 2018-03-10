#pragma once

#include <string>
#include <boost/thread/mutex.hpp>

namespace r7 {


class IoServerContext
{

private:
    IoServerContext(void);
public:
    enum Level {DBG, INF, ERR};
    static const char *LevelNames[];

    static boost::mutex log_mutex;

public:
    ~IoServerContext(void);

    static void init(void);

    static void log(const std::string& msg, Level level);
    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void error(const std::string& msg);
};
}


