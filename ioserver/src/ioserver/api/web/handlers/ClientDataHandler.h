#pragma once

#include "civetweb/CivetServer.h"

namespace r7 {

class ClientDataHandler : public CivetHandler
{
public:
    ClientDataHandler();

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);


    virtual ~ClientDataHandler();

protected:
private:
    bool handGet_test(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info);
    bool handGet_json(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info, const std::string baseFilename);

    std::string getFileNameFromPath(const struct mg_request_info* req_info);
};

}
