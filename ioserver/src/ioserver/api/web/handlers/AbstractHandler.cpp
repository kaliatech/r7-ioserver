#include "AbstractHandler.h"

#include <sstream>

#include "nlohmann-json/json.hpp"
#include "../MimeType.h"
#include "ioserver/IoServerException.h"

namespace r7 {

using json = nlohmann::json;

AbstractHandler::AbstractHandler(const IoServerContext& ctx) :
    ctx(ctx)
{

}

bool AbstractHandler::handleCommonRequests(CivetServer *server, struct mg_connection *conn) {
    return false;
}

bool AbstractHandler::handleError(struct mg_connection *conn, const IoServerException& ex) {
    return this->handleError(conn, ex.getStatusCode(), ex.what());
}

bool AbstractHandler::handleError(struct mg_connection *conn, const int statusCode, const std::string& msg) {


//    HTTP/1.1 500 Internal Server Error
//    Cache-Control: no-cache, no-store, must-revalidate, private, max-age=0
//    Pragma: no-cache
//    Expires: 0
//    Content-Type: text/plain; charset=utf-8
//    Date: Fri, 16 Mar 2018 22:41:08 GMT
//    Connection: close
    // mg_send_http_error(conn, statusCode, "%s", msg.c_str());

    const char *status_text = mg_get_response_code_text(conn, statusCode);

    json errMsgJson;
    errMsgJson["msg"] = msg;

    mg_printf(conn,     "HTTP/1.1 %i %s\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Access-Control-Max-Age: 86400\r\n"
                        "Content-Type: %s; charset=utf-8\r\n"
                        "\r\n%s",
                        statusCode,
                        status_text,
                        MimeTypes.at(MimeType::APPLICATION_JSON).str.c_str(),
                        errMsgJson.dump().c_str());

    return true;
}

std::unique_ptr<std::string> AbstractHandler::readRequestBody(struct mg_connection *conn) {
    std::unique_ptr<std::string> reqBody(new std::string());
    char* buf = new char[1024 * 1024 * 1];
    int bytesRead = mg_read(conn, buf, sizeof(buf));
    while (bytesRead > 0) {
        reqBody->append(buf, bytesRead);
        bytesRead = mg_read(conn, buf, sizeof(buf));
    }
    return reqBody;
}

void AbstractHandler::sendResp(struct mg_connection *conn, int status, MimeType mimeType, std::string reqBody) {

    const std::string &contentType = MimeTypes.at(mimeType).str;

    //    std::stringstream s(std::ios_base::app | std::ios_base::out);
    //    s << "HTTP/1.1 ";
    //    s << std::to_string(status);
    //    s <<  " OK\r\n";
    //    s << "Access-Control-Allow-Origin: *\r\n"
    //         "Access-Control-Max-Age: 86400\r\n"
    //         "Content-Type: " + contentType + "\r\n\r\n";

    //    mg_printf(conn, s.str().c_str());


    mg_printf(conn, "HTTP/1.1 %i OK\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Max-Age: 86400\r\n"
                    "Content-Type: %s\r\n\r\n%s",
                    status, contentType.c_str(), reqBody.c_str());


}

}

