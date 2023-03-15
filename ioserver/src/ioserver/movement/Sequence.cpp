#include "Sequence.h"

#include <memory>

namespace r7 {

using std::shared_ptr;

Sequence::Sequence() {
}

Sequence::Sequence(const nlohmann::json& jsonObj) {
    this->fromJson(jsonObj);
}

const std::string& Sequence::getSequenceId() {
    return this->sequenceId;
}

const std::vector<Action>& Sequence::getActions() {
    return this->actions;
}

void Sequence::fromJson(const nlohmann::json& jsonObj) {
    this->sequenceId = jsonObj.at("id").get<std::string>();

    //TODO: Not thread safe.
    this->actions.clear();

    std::vector<nlohmann::json> actionsJson = jsonObj.at("actions");
    for (auto it = actionsJson.begin(); it != actionsJson.end(); ++it) {
        Action action (*it);
        actions.push_back(action);
    }
}
}
