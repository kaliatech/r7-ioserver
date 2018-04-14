#include "IoServer.h"
#include "civetweb/CivetServer.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <iostream>

#include "data/DatabaseManager.h"
#include "ioserver/IoServerConfig.h"
#include "IoServerContext.h"
#include "ioserver/IoServerException.h"

#include "api/web/WebServerProcess.h"

#include "easyloggingpp/easylogging++.h"

#define DOCUMENT_ROOT "/dev/null"

namespace r7 {

IoServer::IoServer(const IoServerConfig& config)
{
    try {
        // TODO: consider converting these to stack (const references). IoServer is owner
        // and should guarantee references to be valid until everything is stopped, including
        // any threads.s

        dbm = std::make_shared<r7::DatabaseManager>(config);

        cm = std::make_shared<r7::ControllerManager>(dbm);

        sm = std::make_shared<r7::ServoManager>(dbm);

        ctx = std::make_shared<r7::IoServerContext>(dbm, cm, sm);

        //TODO: This is doesn't return. Eventually should move to own thread and have IoServer be the master owner.
        LOG(INFO) << "Listening on port:" << config.getPort();
        this->webSrvr = std::make_unique<r7::WebServerProcess>(config, *ctx);
    }
    catch (const std::exception& e) {
        LOG(ERROR) << "Error initializing. " << e.what();
        system("pause");
    }
    catch (...) {
        LOG(ERROR) << "Error initializing.";
        system("pause");
    }
}

void IoServer::stop() {
    if (webSrvr) {
        webSrvr->stop();
    }
    if (cm) {
        cm->stop();
    }
}
}


