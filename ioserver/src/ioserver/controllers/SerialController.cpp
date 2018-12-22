#include "SerialController.h"

#include <memory>
#include <boost/thread.hpp>

#include "../IoServerContext.h"
#include "../servos/Servo.h"
#include "ioserver/serialport/AsyncSerialPort.h"

#include "easyloggingpp/easylogging++.h"

namespace r7 {

SerialController::SerialController() :
    SerialController("", nullptr)
{
}

SerialController::SerialController(const std::string& type, std::unique_ptr<nlohmann::json> jsonObj) :
    IoController(type, std::move(jsonObj)),
    serialPort(std::make_shared<AsyncSerialPort>())
{

    // TODO: Research this. Important?
    // https://stackoverflow.com/a/6364939/123378
    // https://stackoverflow.com/questions/35651910/waiting-boost-asios-future-lasts-forever-after-io-service-stop
    // https://stackoverflow.com/a/42344001/123378
    // #include <boost/asio/signal_set.hpp>
    // Register signal handlers so that the daemon may be shut down. You may
    // also want to register for other signals, such as SIGHUP to trigger a
    // re-read of a configuration file.
    //    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
    //    signals.async_wait(
    //        boost::bind(&boost::asio::io_service::stop, &io_service));

    //serialPortIOThread = new boost::thread(boost::bind(&AsyncSerialPort::run_io_service), serialPort);
    serialPortIOThread = new boost::thread(&AsyncSerialPort::run_io_service, serialPort);
    serialPort->eventSignal.connect(boost::bind(&SerialController::onSerialPortEvent, this, _1));
}

SerialController::~SerialController()
{
    this->serialPort->close();
    delete serialPortIOThread;
}

void SerialController::reset() {
    if (serialPort->isOpen()) {
        this->serialPort->close();
    }
}

void SerialController::stop() {
    if (serialPort->isOpen()) {
        this->serialPort->close();
    }
    serialPort->stop_io_service();
    serialPortIOThread->join();
}

void SerialController::moveToPulse(const Servo& servo, const int& pulse) {
    if (!serialPort->isOpen()) {
        std::string ioConnStr = this->getIoConnStr();
        LOG(INFO) << "Opening serial port: " << ioConnStr;
        try {
            serialPort->open(ioConnStr,
                             115200,
                             boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                             boost::asio::serial_port_base::character_size(8),
                             boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                             boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        }
        catch(boost::system::system_error e)
        {
            LOG(ERROR) << "Error opening port: " << ioConnStr << ". Msg: " << e.what();
            return;
        }
    }
    //    std::string str = std::string("first test");
    //    std::vector<unsigned char> data(str.begin(), str.end());

    //unsigned char data[4];

    //uint8_t

    std::vector<unsigned char> data(4);

    // set speed
    //0x87, channel number, speed low bits, speed high bits
    int16_t pulsePerMsTenths = 10;

    int16_t speed16 = (int16_t)pulsePerMsTenths * 4;
    data[0] = 0x87; // Command byte: Set Target.
    data[1] = (unsigned char)servo.getPin(); // First data byte holds channel number.
    data[2] = speed16 & 0x7F; // Second byte holds the lower 7 bits of target.
    data[3] = (speed16 >> 7) & 0x7F;   // Third data byte holds the bits 7-13 of target.
    serialPort->write(data);

    // set target
    int16_t pulse16 = (int16_t)pulse * 4;
    data[0] = 0x84; // Command byte: Set Target.
    data[1] = (unsigned char)servo.getPin(); // First data byte holds channel number.
    data[2] = pulse16 & 0x7F; // Second byte holds the lower 7 bits of target.
    data[3] = (pulse16 >> 7) & 0x7F;   // Third data byte holds the bits 7-13 of target.
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
