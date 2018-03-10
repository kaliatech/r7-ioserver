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
#include "api/web/WebServerProcess.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DOCUMENT_ROOT "/dev/null"

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

    r7::WebServerProcess webSrvr(options);

    return 0;
}
