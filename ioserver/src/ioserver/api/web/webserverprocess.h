#pragma once

#include <memory>
#include "../../IoServerContext.h"

#include "civetweb/CivetServer.h"

#include "handlers/ClientDataHandler.h"
#include "handlers/ServoMoveHandler.h"
#include "handlers/ExitHandler.h"

#include <memory>

namespace r7 {

class WebServerProcess
{
public:
    WebServerProcess(const IoServerConfig& config, const IoServerContext& ctx);

    void stop();

private:
    const IoServerContext& ctx;
    std::unique_ptr<CivetServer> server;
    ClientDataHandler clientDataHandler;
    ServoMoveHandler servoMoveHandler;
    ExitHandler exitHandler;
};

}
