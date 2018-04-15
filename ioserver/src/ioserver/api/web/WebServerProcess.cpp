#include "WebServerProcess.h"

#include "civetweb/CivetServer.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

//#include "ioserver/IoController.h"
//#include "ioserver/IoServerConfig.h"
#include "handlers/ClientDataHandler.h"
#include "handlers/ServoMoveHandler.h"
#include "ioserver/IoServerConfig.h"

namespace r7 {

#define DOCUMENT_ROOT "./ui"
#define DATA_URI "/data"
#define EXIT_URI "/exit"

WebServerProcess::WebServerProcess(const IoServerConfig& config, const IoServerContext& ctx) :
    ctx(ctx),
    clientDataHandler(ctx),
    servoMoveHandler(ctx),
    exitHandler()
{
    std::string port(std::to_string(config.getPort()));

    // "url_rewrite_patterns", "/servos=C:/Projects/r7/r7-ioserver-ui/dist/index.html",
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

    server = std::make_unique<CivetServer>(options);

    server->addHandler(DATA_URI, clientDataHandler);

    server->addHandler("/mbp", servoMoveHandler);

    server->addHandler(EXIT_URI, exitHandler);


    // printf("Data at http://localhost:%d%s\n", config.getPort(), DATA_URI);
    // printf("Exit at http://localhost:%d%s\n", config.getPort(), EXIT_URI);

    //TODO: Is there a better way to keep the process open?
    //    while (!exitNow) {
    //      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //    }

}

void WebServerProcess::stop() {
    server->close();
}

}
