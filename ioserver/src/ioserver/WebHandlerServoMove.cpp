#include "ioserver/WebHandlerServoMove.h"

#include "nlohmann-json/json.hpp"
#include <limits>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

// for convenience
using json = nlohmann::json;

WebHandlerServoMove::WebHandlerServoMove()
{
}

bool WebHandlerServoMove::handleGet(CivetServer *server, struct mg_connection *conn)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    if (!strcmp(req_info->request_method, "OPTIONS")) {
        return false;
    }

    return false;
}

bool WebHandlerServoMove::handlePost(CivetServer *server, struct mg_connection *conn) {

    const struct mg_request_info *req_info = mg_get_request_info(conn);

    if (!strcmp(req_info->request_method, "OPTIONS")) {
        return false;
    }

    mg_printf(conn, "HTTP/1.1 201 OK\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Max-Age: 86400\r\n"
        "Content-Type: application/json\r\n\r\n");

    json jsonResp;
    jsonResp["msg"] = "test okay";

    mg_printf(conn, jsonResp.dump(2).c_str());

    return true;
}

WebHandlerServoMove::~WebHandlerServoMove()
{
    //dtor
}
