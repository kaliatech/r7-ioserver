#pragma once

#include "IoController.h"

#include <memory>

#include <boost/thread.hpp>

#include "nlohmann-json/json.hpp"

#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

class SerialController : public IoController {

public:
    SerialController();
    SerialController(const std::string& type, std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~SerialController();

    virtual void moveToPulse(const Servo& servo, const int& pulse);
    virtual void reset();
    virtual void stop();

protected:
    virtual void onSerialPortEvent(boost::shared_ptr<AsyncSerialEvent> evt);

private:
    std::shared_ptr<AsyncSerialPort> serialPort;
    boost::thread* serialPortIOThread; //TODO: Move this back in to the AsyncSerialPort constructor
};


}


