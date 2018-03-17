#include "ioserver/IoServerException.h"

IoServerException::IoServerException(const std::string& msg) :
  std::logic_error(msg),
  statusCode(500)
{
}

IoServerException::IoServerException(const std::string& msg, const int statusCode) :
  std::logic_error(msg),
  statusCode(statusCode)
{
}
