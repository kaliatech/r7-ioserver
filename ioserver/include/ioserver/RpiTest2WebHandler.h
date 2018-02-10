#ifndef RpiTest2WebHandler_H
#define RpiTest2WebHandler_H

#include "civetweb/CivetServer.h"
#include <fstream>
#include <string>

class RpiTest2WebHandler : public CivetHandler
{
    public:
        RpiTest2WebHandler(const std::string& dataFilePath, const unsigned int maxNumlines);

        bool handleGet(CivetServer *server, struct mg_connection *conn);

        virtual ~RpiTest2WebHandler();

    private:
        std::string exec(std::string cmd);

    protected:
    private:
        std::string   mDataFilePath;
        unsigned int  mMaxNumLines;
};

#endif // RpiTest2WebHandler_H
