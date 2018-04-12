#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>
#include <cstdio>//to pause console screen
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


#include "ioserver/IoServerConfig.h"
#include "IoServer.h"

//add_definitions(-DELPP_THREAD_SAFE)
//add_definitions(-DELPP_STL_LOGGING) # TODO: Worthwhile?
//add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
//add_definitions(-DELPP_NO_LOG_TO_FILE)
#include "easyloggingpp/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{

    r7::IoServer* ioServer;
    try {
        r7::IoServerConfig::configureLogging(r7::IoServerConfig::DEFAULT_LOGLEVEL, "");

        r7::IoServerConfig config(argc, argv);
        if (!config.isValid) {
            LOG(ERROR) << "Invalid configuration.";
            return -1;
        }

        LOG(INFO) << "-----------------------";
        LOG(INFO) << "Starting r7-ioserver";
        LOG(INFO) << "-----------------------";

        ioServer = new r7::IoServer(config);
    }
    catch (const std::exception& ex)  {
        LOG(FATAL) << "Error starting server. " << ex.what();
        return -1;
    }
    catch (...)  {
        LOG(FATAL) << "Error starting server.";
        return -1;
    }

    // TODO: Refactor this. Use external triggers and catch signals. Signal handling will have to be OS dependent.
    char ch;
    LOG(INFO) << "Press ENTER to stop...\n";
//    while (std::cin.readsome(&ch, 1) != 0);
//    getchar();

    while (true) {
        int c=getchar();
        if (c=='\n' || c==EOF) break;
    }

    LOG(INFO) << "Stopping.";

    ioServer->stop();

    return 0;
}
