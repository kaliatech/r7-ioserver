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
    if (tokens.size() != 4) {
        return this->handleError(conn, 400, "Invalid request due to wrong number of parameters.");
    }

    //    json jsonResp;
    //    jsonResp["msg"] = "test okay";
    //    jsonResp["request_uri"] = req_info->request_uri;
    //    jsonResp["token0"] = tokens[0]; //empty
    //    jsonResp["mbp"] = tokens[1]; // mbp
    //    jsonResp["servoId"] = tokens[2]; // servoId
    //    jsonResp["servoPulse"] = tokens[3]; // servoPulse

    try {
        std::string servoId = tokens[2];
        int servoPulse = std::stoi(tokens[3]);

        auto servo = ctx.sm->findServoById(servoId);
        if (servo == nullptr) {
            return this->handleError(conn, 404, "Servo not found");
        }

        auto ioController = ctx.cm->findControllerById(servo->getControllerId());
        if (ioController == nullptr) {
            return this->handleError(conn, 404, "No controller for servo");
        }

        ioController->moveToPulse(*servo, servoPulse);

        std::string tmp("");

//        ioController->moveToPulse(servo, servoPulse);

    }catch (const IoServerException& e) {
        return this->handleError(conn, e);
    } catch (const std::exception& ex) {
        return this->handleError(conn, 400, std::string("Invalid request. Reason:") +  ex.what());
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 400, "Invalid request. Reason unknown.");
    }

    return this->sendResp(conn, 204, r7::MimeType::APPLICATION_JSON, "");
}

ServoMoveHandler::~ServoMoveHandler()
{
    delete this->ioController;
}
}
