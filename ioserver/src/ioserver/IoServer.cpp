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
    //printf("Settings\n");
    //printf("  Max IPs    : %d\n", config.getMaxAllowedIps());
    //printf("  Format     : %s\n", config.getFormat().c_str());


    std::string port(std::to_string(config.getPort()));
    const char *options[] = {
        "document_root", DOCUMENT_ROOT,
        "listening_ports", port.c_str(),
        "access_control_allow_origin", "*",
        "access_control_allow_methods", "*",
        "access_control_allow_headers", "*",
        "enable_keep_alive", "yes",
        "keep_alive_timeout_ms", "30000",
        "tcp_nodelay", "1", // TODO: Need to test this
        0
    };

    try {
        //std::shared_ptr<r7::DatabaseManager> db(new r7::DatabaseManager());
        dbm = std::make_shared<r7::DatabaseManager>(config);
        dbm->reinitSchema();

        cm = std::make_shared<r7::ControllerManager>(dbm);

        sm = std::make_shared<r7::ServoManager>(dbm);

        ctx = std::make_shared<r7::IoServerContext>(dbm, cm, sm);

        //TODO: This is doesn't return. Eventually should move to own thread and have IoServer be the master owner.
        LOG(INFO) << "Listening on port:" << port;
        this->webSrvr = std::make_unique<r7::WebServerProcess>(options, *ctx);
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
        this->webSrvr->stop();
    }
    if (cm) {
        cm->stop();
    }
}
}


