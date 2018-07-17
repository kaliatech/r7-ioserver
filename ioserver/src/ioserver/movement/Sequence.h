#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Action.h"

#include "nlohmann-json/json.hpp"
namespace r7 {

class Sequence
{
public:
    Sequence();
    Sequence(const nlohmann::json& jsonObj);
    virtual ~Sequence() = default;

    const std::string& getSequenceId();
    const std::vector<Action>& getActions();

private:
    void fromJson(const nlohmann::json& jsonObj);

private:
    std::string sequenceId;
    std::vector<Action> actions;
};

}
