#include "SerialController.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "../IoServerContext.h"
#include "ioserver/serialport/AsyncSerialPort.h"

namespace r7 {

SerialController::SerialController() :
    serialPort(boost::shared_ptr<AsyncSerialPort>(new AsyncSerialPort()))
{
    serialPortIOThread = new boost::thread(boost::bind(&AsyncSerialPort::run_io_service, serialPort));

    serialPort->eventSignal.connect(boost::bind(&SerialController::onSerialPortEvent, this, _1));

}

void SerialController::doTest() {
    if (!serialPort->isOpen()) {

        try {
            serialPort->open("COM3",
                             115200,
                             boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                             boost::asio::serial_port_base::character_size(8),
                             boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                             boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        }
        catch(boost::system::system_error e)
        {
            std::ostringstream logmsg;
            logmsg << "SerialProxyClient:: Error opening port:";
            logmsg << "COM3";
            logmsg << ". Reason:";
            logmsg << e.what();
            //IoServerContext::error(logmsg.str());
        }
    }
    std::string str = std::string("first test");
    std::vector<unsigned char> data(str.begin(), str.end());
    serialPort->write(data);
}


void SerialController::onSerialPortEvent(boost::shared_ptr<AsyncSerialEvent> evt) {

    switch (evt->getType())
    {

    case AsyncSerialEvent::DATA_READ :
    {
        //MessageFromSerialPort msg(RawDataMessage::RAW_SERIAL_RESP_CODE, evt->getData());
        //MessageFromSerialPort msg(evt->getData());
        break;
    }
    default :
    {
        //CommServerContext::error("Unhandled event code in SerialProxyClient::onSerialPortEvent");
    }

    }

}

}
