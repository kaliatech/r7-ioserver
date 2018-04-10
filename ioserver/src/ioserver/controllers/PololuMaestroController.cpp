#include "PololuMaestroController.h"

#include <memory>

namespace r7 {

PololuMaestroController::PololuMaestroController() :
    PololuMaestroController(nullptr)
{
}

PololuMaestroController::PololuMaestroController(std::unique_ptr<nlohmann::json> jsonObj) :
    SerialController("pololu-maestro", std::move(jsonObj))
{

}


}
