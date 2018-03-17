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

#define DOCUMENT_ROOT "/dev/null"

namespace r7 {

IoServer::IoServer()
{
    IoServerConfig config;
    bool result = config.parseConfig();
    if (!result) {
        printf("Unable to parse config file.");
//        return;
    }

    printf("Settings\n");
    printf("  Max IPs    : %d\n", config.getMaxAllowedIps());
    printf("  Format     : %s\n", config.getFormat().c_str());


    std::string port(std::to_string(config.getPort()));
    const char *options[] = {
        "document_root", DOCUMENT_ROOT,
        "listening_ports", port.c_str(),
        "access_control_allow_origin", "*",
        "access_control_allow_methods", "*",
        "access_control_allow_headers", "*",
        0
    };

    try {
        std::shared_ptr<r7::DatabaseManager> db(new r7::DatabaseManager());
        db->reinitSchema();

        IoServerContext ctx(db);

        //TODO: This is doesn't return. Eventually should move to own thread and have IoServer be the master owner.
        r7::WebServerProcess webSrvr(options, ctx);
    }
    catch (const IoServerException& e) {
        printf("Error initializing. Message: %s\n", e.what());
        system("pause");
    }
}
}


