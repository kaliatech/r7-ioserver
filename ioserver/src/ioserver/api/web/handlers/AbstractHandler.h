#pragma once

#include <memory>
#include <string>
#include "civetweb/CivetServer.h"
#include "../../../IoServerContext.h"
#include "ioserver/IoServerException.h"

#include "../MimeType.h"

namespace r7 {

class AbstractHandler : public CivetHandler
{
public:
    AbstractHandler(const IoServerContext& ctx);

protected:
    const IoServerContext& ctx;

    bool handleCommonRequests(CivetServer *server, struct mg_connection *conn);

    bool AbstractHandler::handleError(struct mg_connection *conn, const IoServerException& ex);
    bool AbstractHandler::handleError(struct mg_connection *conn, const int statusCode, const std::string& msg);

    bool sendResp(struct mg_connection *conn, int status, r7::MimeType contentType, std::string respBody);

    std::unique_ptr<std::string> readRequestBody(struct mg_connection *conn);
};

}
