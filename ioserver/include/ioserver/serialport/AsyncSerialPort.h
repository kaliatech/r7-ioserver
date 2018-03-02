#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "AsyncSerialEvent.h"

namespace r7 {
    namespace ioserver {

class AsyncSerialPort : public boost::enable_shared_from_this<AsyncSerialPort>
{
public:
	AsyncSerialPort(void);
	~AsyncSerialPort(void);

    /**
    * Opens a serial device.
    * @param devname serial device name, example "/dev/ttyS0" or "COM1"
    * @param baud_rate serial baud rate
    * @param opt_parity serial parity, default none
    * @param opt_csize serial character size, default 8bit
    * @param opt_flow serial flow control, default none
    * @param opt_stop serial stop bits, default 1
    * @throws boost::system::system_error if cannot open the serial device
    */
	void open(	const std::string& devname,
				unsigned int baud_rate,
				boost::asio::serial_port_base::parity opt_parity,
				boost::asio::serial_port_base::character_size opt_csize,
				boost::asio::serial_port_base::flow_control opt_flow,
				boost::asio::serial_port_base::stop_bits opt_stop);

	bool isOpen();

    /**
     * Close the serial device
     * @throws boost::system::system_error if any error
     */
    void close();

     /**
     * Write data asynchronously. Returns immediately.
     * @param data to be sent through the serial device
     */
    void write(const std::vector<unsigned char>& data);

	/**
	 * Read data asynchronously. Returns immediately.
	 */
	void read(unsigned int  numBytes);
	
	void run_io_service();

    boost::signals2::signal <void (boost::shared_ptr<AsyncSerialEvent>)>	eventSignal;

	inline boost::shared_ptr<AsyncSerialPort> getSharedPtr()
	{
		return shared_from_this();
	};

private:
	void doAsyncWrites();
	void onAsyncWriteComplete(const boost::system::error_code& error, std::size_t  bytes_transferred);

	boost::function<void (const boost::system::error_code&, size_t)> onAsyncReadFunctor;
	boost::function<void (const boost::system::error_code&, size_t)> onAsyncWriteFunctor;
	
	void doAsyncRead(unsigned int numBytes);
	void onAsyncReadComplete(const boost::system::error_code& error, std::size_t bytes_transferred);


private:
	bool serialPortOpen;

    static const int readBufferSize=512;

	boost::asio::io_service io_service; ///< Io service object
    boost::asio::serial_port port; ///< Serial port object

    /// Data are queued here before they go in writeBuffer
    std::vector<unsigned char> writeQueue;
    boost::shared_array<unsigned char> writeBuffer; ///< Data being written
    size_t writeBufferSize; ///< Size of writeBuffer
    boost::mutex writeQueueMutex; ///< Mutex for access to writeQueue
    
	//unsigned char _readBuffer[AsyncSerialPort::readBufferSize]; ///< data read
	std::vector<unsigned char> _readDataBuffer;


};




	}
}
