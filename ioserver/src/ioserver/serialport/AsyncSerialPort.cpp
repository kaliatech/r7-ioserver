#include "ioserver/serialport/AsyncSerialPort.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

//#include "../CommServerContext.h" //temporary for logging

#include "ioserver/serialport/AsyncSerialEvent.h"


namespace r7 {

AsyncSerialPort::AsyncSerialPort(void) :
    io_service(),
    port(io_service),
    serialPortOpen(false),
    _readDataBuffer(1024),
    writeBufferSize(0)
{

    onAsyncReadFunctor = boost::bind(	&AsyncSerialPort::onAsyncReadComplete,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred);


    onAsyncWriteFunctor = boost::bind(  &AsyncSerialPort::onAsyncWriteComplete,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred);

}


AsyncSerialPort::~AsyncSerialPort(void)
{

    if(isOpen())
    {
        try {
            close();

        } catch(...)
        {
            //Don't throw from a destructor
        }
    }
    io_service.stop();

    eventSignal.disconnect_all_slots();
}

void AsyncSerialPort::open(	const std::string& devname,
                            unsigned int baud_rate,
                            boost::asio::serial_port_base::parity opt_parity,
                            boost::asio::serial_port_base::character_size opt_csize,
                            boost::asio::serial_port_base::flow_control opt_flow,
                            boost::asio::serial_port_base::stop_bits opt_stop)
{


    if(isOpen())
    {
        close();
    }

    //setErrorStatus(true);//If an exception is thrown, error_ remains true
    
    port.open(devname);
    port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    port.set_option(opt_parity);
    port.set_option(opt_csize);
    port.set_option(opt_flow);
    port.set_option(opt_stop);

    //setErrorStatus(false);//If we get here, no error
    
    serialPortOpen=true; //Port is now open

}


void AsyncSerialPort::run_io_service()
{
    boost::asio::io_service::work work(io_service); //keeps service running even if no operations ...alternatively could queue up 1st read operation maybe
    try
    {
        io_service.run();
        //CommServerContext::debug("SerialPort io_service run exiting");

    }
    catch (...)
    {
        //would be better to set an error status
        // CommServerContext::error("Unexpected exception while running serial port io_service.");
        close(); //alternatively, keep running
    }
}


bool AsyncSerialPort::isOpen()
{
    return serialPortOpen;
}

void AsyncSerialPort::close()
{
    serialPortOpen = false;
    port.close();

}

void AsyncSerialPort::write(const std::vector<unsigned char>& data)
{
    if (!isOpen()) return;

    //scope for the lock
    {
        boost::lock_guard<boost::mutex> l(writeQueueMutex);
        writeQueue.insert(writeQueue.end(),data.begin(),data.end());
    }

    io_service.post(boost::bind(&AsyncSerialPort::doAsyncWrites, this));
}


void AsyncSerialPort::doAsyncWrites()
{
    //If a write operation is already in progress, do nothing
    ///TODO: Should this be writeBuffer > 0?
    if(writeBufferSize==0)
    {
        boost::lock_guard<boost::mutex> l(writeQueueMutex);
        writeBufferSize=writeQueue.size();
        writeBuffer.reset(new unsigned char[writeQueue.size()]);
        std::copy(writeQueue.begin(), writeQueue.end(), writeBuffer.get());
        writeQueue.clear();


        boost::asio::async_write(	port,
                                    boost::asio::buffer(writeBuffer.get(),writeBufferSize),
                                    onAsyncWriteFunctor);
    }
}

void AsyncSerialPort::onAsyncWriteComplete(const boost::system::error_code& error, std::size_t bytes_transferred)
{

    if(error)
    {
        if(!isOpen()) return;

        //setErrorStatus(true);
        //doClose();

        return;
    }

    boost::lock_guard<boost::mutex> l(writeQueueMutex);
    if(writeQueue.empty())
    {
        writeBuffer.reset();
        writeBufferSize=0;

        return;
    }
    writeBufferSize=writeQueue.size();
    writeBuffer.reset(new unsigned char[writeQueue.size()]);
    std::copy(writeQueue.begin(), writeQueue.end(), writeBuffer.get());
    writeQueue.clear();
    boost::asio::async_write(	port,
                                boost::asio::buffer(writeBuffer.get(),writeBufferSize),
                                onAsyncWriteFunctor);

}




void AsyncSerialPort::read(unsigned int numBytes)
{
    if (!isOpen()) return;

    io_service.post(boost::bind(&AsyncSerialPort::doAsyncRead, this, numBytes));


}


void AsyncSerialPort::doAsyncRead(unsigned int numBytes)
{

    boost::asio::async_read(port,
                            boost::asio::buffer(_readDataBuffer, numBytes),
                            onAsyncReadFunctor);
}

void AsyncSerialPort::onAsyncReadComplete(const boost::system::error_code& error, std::size_t bytes_transferred)
{

    if(error)
    {
        //error can be true  because the serial port was closed
        if(!isOpen()) return;

#ifdef __APPLE__
        if(error.value()==45)
        {
            //Bug on OS X, it might be necessary to repeat the setup
            //http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
            
            //doRead();
            //return;
        }
#endif //__APPLE__

        //doClose();
        //setErrorStatus(true);

        return;
    }

    std::vector<unsigned char> readData(_readDataBuffer.begin(), _readDataBuffer.begin() + bytes_transferred);

    AsyncSerialEvent* evt = new AsyncSerialEvent(this->getSharedPtr(), AsyncSerialEvent::DATA_READ, readData);

    //notify listeners with buffer contents
    boost::shared_ptr<AsyncSerialEvent> evtPtr = boost::shared_ptr<AsyncSerialEvent>(evt);
    eventSignal(evtPtr);

}



}
