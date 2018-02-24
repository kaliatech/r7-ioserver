/* Copyright (c) 2017 Joshua Sanderson
 * Copyright (c) 2017 JGS Technical LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

#include "civetweb/CivetServer.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

#include "ioserver/IoServerConfig.h"
#include "ioserver/WebHandlerClientData.h"
#include "ioserver/WebHandlerServoMove.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

//#include <boost/asio.hpp>
#include <iostream>

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

//int main(int argc, char *argv[])
//{
//    boost::asio::io_service io_service;
//    for( int x = 0; x < 42; ++x )
//    {
//        io_service.poll();
//        std::cout << "Counter: " << x << std::endl;
//    }
//    return 0;
//}

int main(int argc, char *argv[])
{
    IoServerConfig config;
    bool result = config.parseConfig();
    if (!result) {
        printf("Unable to parse config file.");
        return -1;
    }

    printf("Settings\n");
    printf("  Max IPs    : %d\n", config.getMaxAllowedIps());
    printf("  Format     : %s\n", config.getFormat().c_str());


    std::string port(std::to_string(config.getPort()));
    const char *options[] = {
        "document_root", DOCUMENT_ROOT,
        "listening_ports", port.c_str(),
        "access_control_allow_origin", "*",
        "access_control_allow_methods", "*",
        "access_control_allow_headers", "*",
        0
    };

    CivetServer server(options);

    WebHandlerClientData webHandlerClientData;
    server.addHandler(DATA_URI, webHandlerClientData);

    WebHandlerServoMove webHandlerServoMove;
    server.addHandler("/mbp", webHandlerServoMove);


    ExitHandler h_exit;
    server.addHandler(EXIT_URI, h_exit);

    printf("Data at http://localhost:%d%s\n", config.getPort(), DATA_URI);
    printf("Exit at http://localhost:%d%s\n", config.getPort(), EXIT_URI);

    //TODO: Is there a better way to keep the process open?
    while (!exitNow) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
