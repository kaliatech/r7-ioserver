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

namespace r7 {

#define DOCUMENT_ROOT "/dev/null"
#define DATA_URI "/data"
#define EXIT_URI "/exit"

WebServerProcess::WebServerProcess(const char *options[], const IoServerContext& ctx) :
    ctx(ctx),
    server(options),
    clientDataHandler(ctx),
    servoMoveHandler(ctx),
    exitHandler()
{
//    server = std::make_shared(CivetServer(options));

    server.addHandler(DATA_URI, clientDataHandler);

    server.addHandler("/mbp", servoMoveHandler);

    server.addHandler(EXIT_URI, exitHandler);

    // printf("Data at http://localhost:%d%s\n", config.getPort(), DATA_URI);
    // printf("Exit at http://localhost:%d%s\n", config.getPort(), EXIT_URI);

    //TODO: Is there a better way to keep the process open?
//    while (!exitNow) {
//      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }

}

void WebServerProcess::stop() {
    server.close();
}

}
