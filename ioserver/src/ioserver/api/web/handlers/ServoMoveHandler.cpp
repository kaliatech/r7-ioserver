#include "ServoMoveHandler.h"

#include "nlohmann-json/json.hpp"
#include <limits>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

#include "pystring/pystring.h"

#include "../MimeType.h"

#include "../../../controllers/SerialController.h"

// for convenience
using json = nlohmann::json;

namespace r7 {

ServoMoveHandler::ServoMoveHandler(const IoServerContext& ctx) :
    AbstractHandler(ctx),
    ioController (new SerialController())
{
    // this->ioController = new SerialController();
}

bool ServoMoveHandler::handleGet(CivetServer *server, struct mg_connection *conn)
{
    //    const struct mg_request_info *req_info = mg_get_request_info(conn);

    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }

    return false;
}

bool ServoMoveHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

    const struct mg_request_info *req_info = mg_get_request_info(conn);
    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }



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

    this->sendResp(conn, 201, r7::MimeType::APPLICATION_JSON, jsonResp.dump(2));

    ioController->doTest();

    return true;
}

ServoMoveHandler::~ServoMoveHandler()
{
    delete this->ioController;
}
}
