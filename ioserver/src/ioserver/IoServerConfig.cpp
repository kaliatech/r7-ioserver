#include "ioserver/IoServerConfig.h"


#include <fstream>
#include <map>
#include <sstream>

#include "ioserver/IoServerWebUtils.h"

IoServerConfig::IoServerConfig()
{
    //ctor
}

bool IoServerConfig::parseConfig() {

    //TODO: Should allow the config filename to be passed in as
    //      command line option. For now, hardcoded...
    std::string confFilePath("./ioserver.conf");
    if (FILE *file = fopen(confFilePath.c_str(), "r")) {
        fclose(file);
    } else {
        confFilePath = std::string("/etc/ioserver.conf");
        if (FILE *file = fopen(confFilePath.c_str(), "r")) {
          fclose(file);
        }
        else {
            confFilePath = std::string("");
        }
    }

    std::map<std::string, std::string> settings;

    if (!confFilePath.empty()) {

        std::ifstream file( confFilePath );
        if (file)
        {

            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            std::string line;
            while( std::getline(buffer, line) )
            {
              std::istringstream is_line(line);
              std::string key;
              if( std::getline(is_line, key, '=') )
              {
                std::string value;
                if( std::getline(is_line, value) )
                  settings[r7::trim(key)] = r7::trim(value);
              }
            }
        }
    }

    //Check for user settings, and set defaults if needed
    if ( settings.count("format") == 0) {
        this->mFormat = std::string("nginx");
    } else {
        this->mFormat = std::string(settings["format"]);
    }

    if ( settings.count("maxAllowedIps") == 0) {
        this->mMaxAllowedIps = 50;
    } else {
        this->mMaxAllowedIps = atoi(settings["maxAllowedIps"].c_str());
        if (this->mMaxAllowedIps == 0) {
            return false;
        }
    }

    if ( settings.count("port") == 0) {
        this->mPort = 12080;
    } else {
        this->mPort = atoi(settings["port"].c_str());
        if (this->mPort == 0) {
            return false;
        }
    }

    return true;
}

IoServerConfig::~IoServerConfig()
{
    //dtor
}
