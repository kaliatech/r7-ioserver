#pragma once

#include <string>

namespace r7 {

class IoServerConfig
{
public:
    static const int DEFAULT_PORT;
    static const std::string DEFAULT_CONFIGFILE;
    static const std::string DEFAULT_LOGLEVEL;
    static const std::string DEFAULT_LOGFILEPATH;
    static const std::string DEFAULT_DBPATH;

public:
    static void configureLogging(const std::string& logLevelStr, const std::string& logFilePath);

public:
    IoServerConfig(int argc, char *argv[]);
    virtual ~IoServerConfig();

    uint32_t getPort() const { return mPort; }

    std::string getLogLevel() const {return mLogLevel; }
    std::string getLogFilePath() const {return mLogFilePath; }
    std::string getDbFilePath() const {return mDbFilePath; }

public:
    bool isValid = false;

private:
    uint32_t mPort;
    std::string mLogLevel;
    std::string mLogFilePath;
    std::string mDbFilePath;
};


}
