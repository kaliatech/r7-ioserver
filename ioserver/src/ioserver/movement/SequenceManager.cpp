#include "SequenceManager.h"

#include <memory>
#include "../data/DatabaseManager.h"

#include "nlohmann-json/json.hpp"

#include "easyloggingpp/easylogging++.h"

#include <boost/thread.hpp>

namespace r7 {

using std::shared_ptr;

SequenceManager::SequenceManager(const shared_ptr<DatabaseManager> dbm) :
    dbm(dbm),
    processThread(boost::thread(boost::bind(&SequenceManager::processLoop, this)))
{

}

std::shared_ptr<Sequence> SequenceManager::findById(const std::string& sequenceId) const{
    //TODO: eventually we should cache. For now, look up every time:
    std::unique_ptr<nlohmann::json> jsonObj = this->dbm->findById("sequences", sequenceId);
    if (jsonObj == nullptr) {
        return nullptr;
    }

    //std::shared_ptr<Sequence> sequence = std::make_shared<Sequence>(std::move(jsonObj));
    std::shared_ptr<Sequence> sequence = std::make_shared<Sequence>(*jsonObj.get());
    return sequence;
}


void SequenceManager::play(const std::string& sequenceId, const float speed) {
    LOG(INFO) << "Play:" << sequenceId;

    std::unique_ptr<nlohmann::json> jsonObj = this->dbm->findById("sequences", sequenceId);
    if (jsonObj == nullptr) {
        return;
    }

    Sequence seq(*jsonObj);
    LOG(INFO) << "Okay:" << seq.getSequenceId();

    const std::string& seqId = seq.getSequenceId();
    const std::vector<Action>& actions = seq.getActions();

    for (auto it = actions.begin(); it != actions.end(); ++it) {
        LOG(INFO) << "Time:" << it->time;
    }
}

void SequenceManager::processLoop() {
    LOG(INFO) << "processLoop";
}

}
