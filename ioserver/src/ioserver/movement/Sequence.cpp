#include "Sequence.h"

#include <memory>

namespace r7 {

using std::shared_ptr;

Sequence::Sequence() :
    jsonObj(nullptr) {
}

Sequence::Sequence(std::unique_ptr<nlohmann::json> jsonObj) :
    jsonObj(std::move(jsonObj)) {
}

std::string Sequence::getSequenceId() const {
    return this->jsonObj->at("sequenceId");
}

}
