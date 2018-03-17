#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "IoServer.h"

int main(int argc, char *argv[])
{
    r7::IoServer* ioServer = new r7::IoServer();
    return 0;
}
