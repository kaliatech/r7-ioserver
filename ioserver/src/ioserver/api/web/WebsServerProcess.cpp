#include "WebServerProcess.h"

#include "civetweb/CivetServer.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

//#include "ioserver/IoController.h"
//#include "ioserver/IoServerConfig.h"
#include "handlers/ClientDataHandler.h"
#include "handlers/ServoMoveHandler.h"

namespace r7 {

#define DOCUMENT_ROOT "/dev/null"
#define DATA_URI "/data"
#define EXIT_URI "/exit"

bool exitNow = false;

class ExitHandler : public CivetHandler
{
  public:
    bool
    handleGet(CivetServer *server, struct mg_connection *conn)
    {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
        mg_printf(conn, "Bye!\n");
        exitNow = true;
        return true;
    }
};


WebServerProcess::WebServerProcess(const char *options[])
{
    CivetServer server(options);

    ClientDataHandler ClientDataHandler;
    server.addHandler(DATA_URI, ClientDataHandler);

    ServoMoveHandler ServoMoveHandler;
    server.addHandler("/mbp", ServoMoveHandler);

    ExitHandler h_exit;
    server.addHandler(EXIT_URI, h_exit);

    // printf("Data at http://localhost:%d%s\n", config.getPort(), DATA_URI);
    // printf("Exit at http://localhost:%d%s\n", config.getPort(), EXIT_URI);

    //TODO: Is there a better way to keep the process open?
    while (!exitNow) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }



}

}
