#pragma once

#include <boost/shared_ptr.hpp>
#include <string>

class AsyncSerialEvent;

namespace r7 {


class IoController
{
public:
    IoController();
    virtual ~IoController() = default;

    virtual void doTest() = 0;

public:
    const std::string& getId() {return id;};

private:
    const std::string id;
};

}
