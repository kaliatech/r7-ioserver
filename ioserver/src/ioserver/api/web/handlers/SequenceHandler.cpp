#include "SequenceHandler.h"

#include "nlohmann-json/json.hpp"
#include <limits>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

#include "pystring/pystring.h"

#include "../MimeType.h"

#include "../../../movement/SequenceManager.h"

// for convenience
using json = nlohmann::json;

namespace r7 {

SequenceHandler::SequenceHandler(const IoServerContext& ctx) :
    AbstractHandler(ctx)
{
    // this->ioController = new SerialController();
}

bool SequenceHandler::handleGet(CivetServer *server, struct mg_connection *conn)
{
    //    const struct mg_request_info *req_info = mg_get_request_info(conn);

    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }

    return false;
}

bool SequenceHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

    const struct mg_request_info *req_info = mg_get_request_info(conn);
    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }

    std::vector<std::string> tokens;
    pystring::split(req_info->request_uri, tokens, "/");
    if (tokens.size() != 4) {
        return this->handleError(conn, 400, "Invalid request due to wrong number of parameters.");
    }

    try {
        std::string sequenceId = tokens[2];
        std::string action = tokens[3];

        //TODO: extract speed from params

        if (action == "play") {
            this->ctx.seqm->play(sequenceId, 1.0);
        }

    }catch (const IoServerException& e) {
        return this->handleError(conn, e);
    } catch (const std::exception& ex) {
        return this->handleError(conn, 500, std::string("Invalid request. Reason:") +  ex.what());
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 500, "Invalid request. Reason unknown.");
    }

    return this->sendResp(conn, 204, r7::MimeType::APPLICATION_JSON, "");
}

SequenceHandler::~SequenceHandler()
{
}
}
