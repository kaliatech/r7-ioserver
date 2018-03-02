#include "ioserver/serialport/AsyncSerialEvent.h"


#include <boost/shared_ptr.hpp>
#include "ioserver/serialport/AsyncSerialPort.h"


namespace r7 {
    namespace ioserver {

AsyncSerialEvent::AsyncSerialEvent(boost::shared_ptr<AsyncSerialPort> serialPort, const Type type) :
	_serialPort(serialPort),
	_type(type),
	_data()
{
}

AsyncSerialEvent::AsyncSerialEvent(boost::shared_ptr<AsyncSerialPort> serialPort, const Type type, const std::vector<unsigned char> data) :
_serialPort(serialPort),
_type(type),
_data(data)
{
}


AsyncSerialEvent::~AsyncSerialEvent(void)
{
}


const AsyncSerialEvent::Type AsyncSerialEvent::getType()
{
	return _type;
}


const boost::shared_ptr<AsyncSerialPort> AsyncSerialEvent::getSerialPort()
{
	return _serialPort;
}

const std::vector<unsigned char>& AsyncSerialEvent::getData() const
{
	return _data;
}

}}
