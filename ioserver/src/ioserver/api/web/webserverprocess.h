#pragma once

#include "../../IoServerContext.h"

namespace r7 {

class WebServerProcess
{
public:
    WebServerProcess(const char *options[], IoServerContext& ctx);

private:
    IoServerContext& ctx;
};

}
