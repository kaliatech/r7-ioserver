#ifndef WebHandlerServoMove_H
#define WebHandlerServoMove_H

#include "civetweb/CivetServer.h"

#include "ioserver/IoController.h"

class WebHandlerServoMove : public CivetHandler
{
public:
    WebHandlerServoMove();

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);


    virtual ~WebHandlerServoMove();

protected:
private:
    r7::ioserver::IoController ioController;
};

#endif // WebHandlerServoMove_H
