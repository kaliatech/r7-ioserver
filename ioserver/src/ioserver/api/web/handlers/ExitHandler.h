#pragma once

namespace r7 {

class ExitHandler : public CivetHandler
{
  public:
    ~ExitHandler() = default;
    bool handleGet(CivetServer *server, struct mg_connection *conn)
    {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
        mg_printf(conn, "Bye!\n");
        return true;
    }
};

}
