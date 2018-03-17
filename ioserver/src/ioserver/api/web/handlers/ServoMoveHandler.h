#pragma once

#include "AbstractHandler.h"
#include "../../../controllers/IoController.h"
#include "../../../IoServerContext.h"

namespace r7 {


class ServoMoveHandler : public AbstractHandler
{
public:
    ServoMoveHandler(const IoServerContext& ctx);

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);

    virtual ~ServoMoveHandler();

protected:
private:
    IoController* ioController;
};

}
