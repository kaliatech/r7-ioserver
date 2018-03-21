#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "IoServer.h"

#define ELPP_STL_LOGGING 1 //TODO: Worthwhile?
#define ELPP_THREAD_SAFE 1
#include "easyloggingpp/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

void configureLogging()
{
//    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    //defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %loc %msg ");

    defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
    defaultConf.setGlobally(el::ConfigurationType::Filename, "r7-ioserver.log");
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level [%logger] %msg");

    el::Loggers::setDefaultConfigurations(defaultConf, true);
    //    el::Loggers::reconfigureAllLoggers(defaultConf);

//    el::Logger* ioServerLogger = el::Loggers::getLogger("ioserver");
//    ioServerLogger->info("Starting r7-ioserver 3");

//    CLOG(INFO, "ioserver") << "Starting r7-ioserver 4b";

//    el::Logger* defaultLogger = el::Loggers::getLogger("default");
//    defaultLogger->info("Starting r7-ioserver 1");

//    LOG(INFO) << "Starting r7-ioserver 2";

//    el::Logger* ioServerLogger = el::Loggers::getLogger("ioserver");
//    ioServerLogger->info("Starting r7-ioserver 3");

//    CLOG(INFO, "ioserver") << "Starting r7-ioserver 4b";

}



int main(int argc, char *argv[])
{
    configureLogging();
    LOG(INFO) << "-----------------------";
    LOG(INFO) << "Starting r7-ioserver";
    LOG(INFO) << "-----------------------";
    r7::IoServer* ioServer = new r7::IoServer();
    return 0;
}
