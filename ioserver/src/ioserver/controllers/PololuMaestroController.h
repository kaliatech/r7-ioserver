#pragma once

#include "SerialController.h"

namespace r7 {

class PololuMaestroController : public SerialController
{
public:
    PololuMaestroController();
    virtual ~PololuMaestroController() = default;
};

}
