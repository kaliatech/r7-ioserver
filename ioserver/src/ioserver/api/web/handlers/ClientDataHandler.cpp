#include "ClientDataHandler.h"

#include "nlohmann-json/json.hpp"
#include <limits>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

#include "pystring/pystring.h"

#include "ioserver/IoServerException.h"


namespace r7 {

// for convenience
using json = nlohmann::json;

ClientDataHandler::ClientDataHandler(const IoServerContext& ctx) :
    AbstractHandler(ctx)
{
}

bool ClientDataHandler::handleGet(CivetServer *server, struct mg_connection *conn)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }

    std::vector<std::string> tokens;
    pystring::split(req_info->request_uri, tokens, "/");
    if (tokens.size() != 3 && tokens.size() != 4) {
        return this->handleError(conn, 400, "Invalid request due to wrong number of parameters.");
    }
    std::string type = tokens[2];

    try {

        if (tokens.size() == 3) {
            json jsonResp;
            auto objStrs = this->ctx.dbm->findAll(type);
            for(auto it = objStrs.begin(); it != objStrs.end(); it++ ) {
                jsonResp.push_back(json::parse(*it));
            }
            this->sendResp(conn, 200, r7::MimeType::APPLICATION_JSON, jsonResp.dump());
        }
        else if (tokens.size() == 4) {
            std::string id = tokens[3];
            auto jsonResp = this->ctx.dbm->findById(type, id);
            if (jsonResp->empty()) {
                throw IoServerException("Not Found", 404);
            }
            this->sendResp(conn, 200, r7::MimeType::APPLICATION_JSON, jsonResp->dump());
        }


    }
    //    catch (json::parse_error &e)
    //    {
    //        return this->handleError(conn, 500, e.what());
    //    }
    catch (const IoServerException& e) {
        return this->handleError(conn, e);
    } catch (const std::exception& ex) {
        return this->handleError(conn, 500, ex.what());
    } catch (const std::string& exStr) {
        return this->handleError(conn, 500, exStr);
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 500, "Unknown");
    }

    return true;
}
bool ClientDataHandler::handleDelete(CivetServer *server, struct mg_connection *conn) {
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    std::vector<std::string> tokens;
    pystring::split(req_info->request_uri, tokens, "/");

    if (tokens.size() != 4) {
        return this->handleError(conn, 400, "Invalid request due to wrong number of parameters.");
    }

    std::string type = tokens[2];
    std::string id = tokens[3];

    try {
        this->ctx.dbm->deleteObj(type, id);
        this->sendResp(conn, 204, r7::MimeType::APPLICATION_JSON, "");
    }
    catch (const IoServerException& e) {
        return this->handleError(conn, e);
    } catch (const std::exception& ex) {
        return this->handleError(conn, 500, ex.what());
    } catch (const std::string& exStr) {
        return this->handleError(conn, 500, exStr);
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 500, "Unknown");
    }

    return true;
}

bool ClientDataHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

    const struct mg_request_info *req_info = mg_get_request_info(conn);

    //    if (!strcmp(req_info->request_method, "OPTIONS")) {
    //        return false;
    //    }

    std::vector<std::string> tokens;
    pystring::split(req_info->request_uri, tokens, "/");

    if (tokens.size() != 3 && tokens.size() != 4) {
        return this->handleError(conn, 400, "Invalid request due to wrong number of parameters.");
    }

    std::string type = tokens[2];


    json reqJson;
    std::string prevId;
    std::string newId;
    try {
        std::unique_ptr<std::string> reqBody = this->readRequestBody(conn);
        reqJson = json::parse(*reqBody.get());

        if (tokens.size() == 3) {
            //todo: creating new, extract from request or generate new
            newId = reqJson["id"].get<std::string>();
            if (newId.empty()) {
                newId = "todo-new";
            }
            prevId = newId;
        }
        else {
            // patch controller, possibly changing the id
            prevId = tokens[3];
            newId = reqJson["id"].get<std::string>();
        }
        //    catch (json::parse_error &e)
        //    {
        //        std::cerr << e.what() << std::endl;
        //        jsonResp["msg"] = e.what();
        //        return this->handleError(conn, 500, jsonResp.dump());
        //    }
    } catch (const std::exception& ex) {
        return this->handleError(conn, 400, std::string("Invalid request. Reason:") +  ex.what());
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 400, "Invalid request. Reason unknown.");
    }

    try {
        this->ctx.dbm->save(type, prevId, newId, reqJson);
        auto jsonResp = this->ctx.dbm->findById(type, newId);
        if (jsonResp->empty()) {
            throw IoServerException("Error saving to DB.");
        }
        this->sendResp(conn, 201, r7::MimeType::APPLICATION_JSON, jsonResp->dump());
    }
    catch (const IoServerException& e) {
        return this->handleError(conn, e);
    } catch (const std::exception& ex) {
        return this->handleError(conn, 500, ex.what());
    } catch (const std::string& exStr) {
        return this->handleError(conn, 500, exStr);
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        return this->handleError(conn, 500, "Unknown");
    }

    return true;
}

ClientDataHandler::~ClientDataHandler()
{
    //dtor
}
}
