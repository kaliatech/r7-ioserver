#pragma once

#include <memory>
#include <string>

#include "nlohmann-json/json.hpp"
namespace r7 {

class Sequence
{
public:
    Sequence();
    Sequence(std::unique_ptr<nlohmann::json> jsonObj);
    virtual ~Sequence() = default;

public:
    std::string getSequenceId() const;

private:
    std::unique_ptr<nlohmann::json> jsonObj;
};

}
