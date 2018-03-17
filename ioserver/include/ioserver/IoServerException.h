#pragma once

#include <exception>
#include <string>


class IoServerException : public std::logic_error
{
public:
    IoServerException(const std::string& msg);
    IoServerException(const std::string& msg, const int statusCode);
    virtual ~IoServerException() throw (){}

    const int getStatusCode() const {return this->statusCode;};

private:
    const int statusCode;
};
