#include "SequenceManager.h"

#include <memory>
#include "../data/DatabaseManager.h"

#include "nlohmann-json/json.hpp"

namespace r7 {

using std::shared_ptr;

SequenceManager::SequenceManager(const shared_ptr<DatabaseManager> dbm) :
    dbm(dbm)
{
}

std::shared_ptr<Sequence> SequenceManager::findById(const std::string& sequenceId) const{
    //TODO: eventually we should cache. For now, look up every time:
    std::unique_ptr<nlohmann::json> jsonObj = this->dbm->findById("sequences", sequenceId);
    if (jsonObj == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Sequence> sequence = std::make_shared<Sequence>(std::move(jsonObj));
    return sequence;
}


void SequenceManager::play(const std::string& sequenceId, const float speed) {

}

}
