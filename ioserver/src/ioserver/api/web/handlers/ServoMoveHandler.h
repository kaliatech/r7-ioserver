#pragma once

#include "AbstractHandler.h"
#include "../../../controllers/IoController.h"

namespace r7 {


class ServoMoveHandler : public AbstractHandler
{
public:
    ServoMoveHandler();

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);

    virtual ~ServoMoveHandler();

protected:
private:
    IoController* ioController;
};

}
