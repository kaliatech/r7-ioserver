#pragma once

#include <string>
#include "civetweb/CivetServer.h"

#include "../MimeType.h"

namespace r7 {


class AbstractHandler : public CivetHandler
{
public:
    AbstractHandler();

protected:
    bool handleCommonRequests(CivetServer *server, struct mg_connection *conn);

    void printCommonHeaders(struct mg_connection *conn, int status, r7::MimeType contentType);
};

}
