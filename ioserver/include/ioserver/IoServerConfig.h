#ifndef IoServerConfig_H
#define IoServerConfig_H

#include <string>

class IoServerConfig
{
    public:
        IoServerConfig();
        virtual ~IoServerConfig();
        bool parseConfig();
        std::string getFormat() { return mFormat; }
        void setFormat(std::string val) { mFormat = val; }
        int32_t getMaxAllowedIps() { return mMaxAllowedIps; }
        void setMaxAllowedIps(int32_t val) { mMaxAllowedIps = val; }
        uint32_t getPort() { return mPort; }
        void setPort(uint32_t port) { mPort = port; }
    protected:
    private:
        std::string mFormat;
        int32_t mMaxAllowedIps;
        uint32_t mPort;
};

#endif
