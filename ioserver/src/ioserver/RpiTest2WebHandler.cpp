#include "ioserver/RpiTest2WebHandler.h"
#include <limits>

RpiTest2WebHandler::RpiTest2WebHandler(const std::string& dataFilePath, const unsigned int maxNumLines) :
    mDataFilePath(dataFilePath), mMaxNumLines(maxNumLines)
{
    if (mMaxNumLines < 0) {
        mMaxNumLines = std::numeric_limits<unsigned int>::max();
    }
}

bool RpiTest2WebHandler::handleGet(CivetServer *server, struct mg_connection *conn)
{
    /* Handler may access the request info using mg_get_request_info */
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>Knock</h2>");
//    mg_printf(conn,
//              "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>",
//              req_info->request_method,
//              req_info->uri,
 //             req_info->remote_addr);


    //poxy_set_header X-Real-IP $remote_addr;
    //proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

    const char* remoteIpChar = server->getHeader(conn, std::string("X-Real-IP"));
    if (remoteIpChar == NULL) {
      remoteIpChar = server->getHeader(conn, std::string("X-Forwarded-For"));
    }
    if (remoteIpChar == NULL) {
      remoteIpChar = req_info->remote_addr;
    }
    std::string remoteIp = remoteIpChar == NULL ? std::string() : std::string(remoteIpChar);

    if (remoteIp.empty()) {
        mg_printf(conn,
                  "<p>Could not found remote IP.</p>");

    }
    else {

        mg_printf(conn,
                  "<p>Adding this IP to list: <pre>%s</pre></p>",
                    remoteIp.c_str());

        std::string outputLine = "allow " + remoteIp + ";";

        std::ifstream inFile;
        inFile.open(mDataFilePath.c_str());
        std::vector<std::string> lines;
        std::string line;
        while(std::getline(inFile, line))
        {
            if (line != outputLine) {
                lines.push_back(line);
            }
        }
        inFile.close();

        lines.push_back(outputLine);

        if (lines.size() > mMaxNumLines) {
            lines.erase(lines.begin(), lines.begin() + (lines.size() - mMaxNumLines));
        }

        std::ofstream outFile;
        outFile.open(mDataFilePath.c_str());
        if (outFile.fail()) {
          printf("%s", "failed");
          mg_printf(conn,
                    "<p>Unable to open output file.</p>");

        }
        else {


            for(std::string str : lines) {
        //        if (str.find("#") == 0 ||
        //            str.find_first_not_of(' ') != std::string::npos) {


        //        }
                //printf("line:%s\n", str.c_str());
                //cout << "i = " << str << endl;
                //outFile.write(str.c_str(), str.size());
                outFile << str << std::endl;
            }
            outFile.flush();
            outFile.close();

            mg_printf(conn, "<p>Total lines:%zu</p>", lines.size());
        }
    }



    //TODO: Make server reload command configurable (and optional)
    std::string cmdOutput = this->exec("/usr/bin/sudo /usr/bin/service nginx reload 2>&1");

    mg_printf(conn, "<p>Nginx status: %s</p>", cmdOutput.c_str());

    mg_printf(conn, "</body></html>\n");


    return true;
}

std::string RpiTest2WebHandler::exec(std::string cmd)
{
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    _pclose(pipe);
    return result;
}

RpiTest2WebHandler::~RpiTest2WebHandler()
{
    //dtor
}
