#pragma once

#include "IoController.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

class SerialController : public IoController {

public:
    SerialController();
    virtual ~SerialController() = default;

    virtual void doTest();

protected:
    virtual void onSerialPortEvent(boost::shared_ptr<AsyncSerialEvent> evt);

private:
    boost::shared_ptr<AsyncSerialPort> serialPort;
    boost::thread* serialPortIOThread; //TODO: Move this back in to the AsyncSerialPort constructor
};


}


