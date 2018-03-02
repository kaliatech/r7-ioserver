#include "ioserver/WebHandlerServoMove.h"

#include "nlohmann-json/json.hpp"
#include <limits>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

#include "pystring/pystring.h"

// for convenience
using json = nlohmann::json;

WebHandlerServoMove::WebHandlerServoMove() :
    ioController()
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

    std::vector<std::string> tokens;
    pystring::split(req_info->request_uri, tokens, "/");

    //TODO: Write generic error handler
    //TODO: Error if not tokens.length == 4

    json jsonResp;
    jsonResp["msg"] = "test okay";
    jsonResp["request_uri"] = req_info->request_uri;
    jsonResp["token0"] = tokens[0]; //empty
    jsonResp["token1"] = tokens[1]; // mbp
    jsonResp["token2"] = tokens[2]; // servoId
    jsonResp["token3"] = tokens[3]; // servoPulse

    mg_printf(conn, jsonResp.dump(2).c_str());

    ioController.doTest();

    return true;
}

WebHandlerServoMove::~WebHandlerServoMove()
{
    //dtor
}
