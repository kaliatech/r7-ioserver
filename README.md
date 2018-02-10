# r7-ioserver
A c/c++ server with minimal dependencies. Build-able on Windows, Linux, OSX and Raspberry Pi. Provides HTTP, websocket, and raw TCP/IP APIs to broker serial communications and low-level device access to remote clients.  Runs master controllers, servo controllers, and other routines where performance and proximity to the hardware is important.  Uses [civetweb](https://github.com/civetweb/civetweb) for web protocol handling and [asio](https://think-async.com/) for serial port communications.
 
Hosts the [r7-ioserver-ui](https://github.com/kaliatech/r7-ioserver-ui) web interface. 

Submodule of the [r7 project](https://github.com/kaliatech/r7).
