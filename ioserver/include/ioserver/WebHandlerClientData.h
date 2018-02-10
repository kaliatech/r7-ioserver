#ifndef WebHandlerClientData_H
#define WebHandlerClientData_H

#include "civetweb/CivetServer.h"

class WebHandlerClientData : public CivetHandler
{
public:
    WebHandlerClientData();

    bool handleGet(CivetServer *server, struct mg_connection *conn);
    bool handlePost(CivetServer *server, struct mg_connection *conn);


    virtual ~WebHandlerClientData();

protected:
private:
    bool handGet_test(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info);
    bool handGet_json(CivetServer* server, struct mg_connection* conn, const struct mg_request_info* req_info, const std::string baseFilename);

    std::string getFileNameFromPath(const struct mg_request_info* req_info);
};

#endif // WebHandlerClientData_H
