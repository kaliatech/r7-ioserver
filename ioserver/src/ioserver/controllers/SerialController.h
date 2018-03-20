#pragma once

#include "IoController.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "nlohmann-json/json.hpp"

#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

class SerialController : public IoController {

public:
    SerialController();
    SerialController(std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~SerialController();

    virtual void moveToPulse(const Servo& servo, const int& pulse);

protected:
    virtual void onSerialPortEvent(boost::shared_ptr<AsyncSerialEvent> evt);

private:
    boost::shared_ptr<AsyncSerialPort> serialPort;
    boost::thread* serialPortIOThread; //TODO: Move this back in to the AsyncSerialPort constructor
};


}


