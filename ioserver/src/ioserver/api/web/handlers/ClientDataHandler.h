#pragma once

#include "civetweb/CivetServer.h"
#include "AbstractHandler.h"
#include "../../../IoServerContext.h"

namespace r7 {

class ClientDataHandler : public AbstractHandler
{
public:
    ClientDataHandler(const IoServerContext& ctx);

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);
    bool handleDelete(CivetServer *server, struct mg_connection *conn);


    virtual ~ClientDataHandler();

protected:
private:

    bool handGet_test(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info);
    bool handGet_json(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info, const std::string baseFilename);

    std::string getFileNameFromPath(const struct mg_request_info* req_info);
};

}
