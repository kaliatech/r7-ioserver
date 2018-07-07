#pragma once

#include <memory>

#include "ioserver/IoServerConfig.h"

#include "api/web/WebServerProcess.h"
#include "data/DatabaseManager.h"
#include "controllers/ControllerManager.h"
#include "movement/SequenceManager.h"
#include "servos/ServoManager.h"

namespace r7 {

class IoServer {
public:
    IoServer(const IoServerConfig& config);
    ~IoServer() = default;

    void stop();

private:
    std::shared_ptr<r7::DatabaseManager> dbm;
    std::shared_ptr<r7::ControllerManager> cm;
    std::shared_ptr<r7::SequenceManager> seqm;
    std::shared_ptr<r7::ServoManager> sm;

    std::shared_ptr<r7::IoServerContext> ctx;

    std::unique_ptr<r7::WebServerProcess> webSrvr;
};
}


