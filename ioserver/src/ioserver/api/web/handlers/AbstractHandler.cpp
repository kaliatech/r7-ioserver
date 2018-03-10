#include "AbstractHandler.h"

#include <sstream>

#include "../MimeType.h"

namespace r7 {

AbstractHandler::AbstractHandler()
{

}

bool AbstractHandler::handleCommonRequests(CivetServer *server, struct mg_connection *conn) {
    return false;
}

void AbstractHandler::printCommonHeaders(struct mg_connection *conn, int status, MimeType mimeType) {

    const std::string &contentType = MimeTypes.at(mimeType).str;

    std::stringstream s;
    s << "HTTP/1.1 " << std::to_string(status) <<  " OK\r\n";
    s << "Access-Control-Allow-Origin: *\r\n"
         "Access-Control-Max-Age: 86400\r\n"
         "Content-Type: " + contentType + "\r\n\r\n";


    std::string s2 = std::string("HTTP/1.1 ") + std::to_string(status) + "OK\r\n";

    //    mg_printf(conn, "HTTP/1.1 " + std::to_string(status).c_str() + " OK\r\n"
    //                    "Access-Control-Allow-Origin: *\r\n"
    //                    "Access-Control-Max-Age: 86400\r\n"
    //                    "Content-Type: application/json\r\n\r\n");

    mg_printf(conn, s.str().c_str());
}

}
