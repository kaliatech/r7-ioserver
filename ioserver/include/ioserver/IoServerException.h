#pragma once

#include <stdexcept>
#include <string>


class IoServerException : public std::logic_error
{
public:
    using std::logic_error::logic_error;
    IoServerException(const std::string& msg);
    IoServerException(const std::string& msg, const int statusCode);
    virtual ~IoServerException() throw (){}

    const int getStatusCode() const {return this->statusCode;};

private:
    const int statusCode = 500;
};
