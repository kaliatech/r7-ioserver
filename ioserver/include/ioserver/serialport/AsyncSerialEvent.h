#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>



namespace r7 {


class AsyncSerialPort;

class AsyncSerialEvent
{
public:
	enum Type {
		PORT_DISCONNECT,
		PORT_ERROR,
		DATA_READ
	};

public:
	AsyncSerialEvent(boost::shared_ptr<AsyncSerialPort> serialPortPtr, const Type type);
	AsyncSerialEvent(boost::shared_ptr<AsyncSerialPort> serialPortPtr, const Type type, const std::vector<unsigned char> data);

	virtual ~AsyncSerialEvent(void);

	const Type getType();
	const boost::shared_ptr<AsyncSerialPort> getSerialPort();
	const std::vector<unsigned char>& getData() const;

private:
	const Type _type;
	const boost::shared_ptr<AsyncSerialPort> _serialPort;
	const std::vector<unsigned char> _data;

};


}

