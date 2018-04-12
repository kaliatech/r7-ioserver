#pragma once

#include <memory>
#include "../../IoServerContext.h"

#include "civetweb/CivetServer.h"

#include "handlers/ClientDataHandler.h"
#include "handlers/ServoMoveHandler.h"
#include "handlers/ExitHandler.h"


namespace r7 {

class WebServerProcess
{
public:
    WebServerProcess(const char *options[], const IoServerContext& ctx);

    void stop();

private:
    const IoServerContext& ctx;
    CivetServer server;
    ClientDataHandler clientDataHandler;
    ServoMoveHandler servoMoveHandler;
    ExitHandler exitHandler;
};

}
