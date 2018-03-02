#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

// #include "../common/CommonClient.h"
#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {
    namespace ioserver {

class IoController
{
public:
    IoController();

    void doTest();

private:
    virtual void onSerialPortEvent(boost::shared_ptr<AsyncSerialEvent> evt);

private:
    boost::shared_ptr<AsyncSerialPort> serialPort;
    boost::thread* serialPortIOThread; //TODO: Move this back in to the AsyncSerialPort constructor
};


    }
}
