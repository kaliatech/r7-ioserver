#include "SequenceManager.h"

#include <memory>
#include "../data/DatabaseManager.h"

#include "nlohmann-json/json.hpp"

#include "easyloggingpp/easylogging++.h"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace r7 {

using std::shared_ptr;

SequenceManager::SequenceManager(const std::shared_ptr<DatabaseManager> dbm,
                                 const std::shared_ptr<ServoManager> sm,
                                 const std::shared_ptr<ControllerManager> cm) :
    dbm(dbm),
    sm(sm),
    cm(cm),
    processThread(std::thread(&SequenceManager::processLoop, this))
{

}

SequenceManager::~SequenceManager() {
    running = false;
    std::lock_guard<std::mutex> lk(m);
    sequenceQueue.push(std::string("")); //todo: should be better way of doing this
    cv.notify_one();
    processThread.join();
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

    // Acquire lock then add to queue
    {
        std::lock_guard<std::mutex> lk(m);
        sequenceQueue.push(std::string(sequenceId));
    }
    cv.notify_one();
}

void SequenceManager::processLoop() {

    while (running) {


        std::string activeSequenceId;
        // Wait for notify
        {
            std::unique_lock<std::mutex> lk(m);

            int numElements = sequenceQueue.size();

            if (sequenceQueue.empty()) {
                cv.wait(lk, [this]{return !sequenceQueue.empty();});
                if (!running) {
                    return;
                }
            }
            activeSequenceId = std::string(sequenceQueue.front());
            sequenceQueue.pop();
        }

        std::unique_ptr<nlohmann::json> jsonObj = dbm->findById("sequences", activeSequenceId);
        if (jsonObj == nullptr) {
            return;
        }
        Sequence seq(*jsonObj);
        const std::vector<Action>& actions = seq.getActions();

        LOG(INFO) << "Play Sequence:" << seq.getSequenceId();

        auto start = std::chrono::system_clock::now();
        //auto start_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

        long msElapsedVal = 0;
        for (auto it = actions.begin(); it != actions.end(); ++it) {

            long waitTime = it->time - msElapsedVal;
            if (waitTime > 0 ) {
                std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
            }

            auto now = std::chrono::system_clock::now();
            auto msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            msElapsedVal = msElapsed.count();

            LOG(INFO) << "Scheduled:" << it->time << " - " << "Elapsed:" << msElapsedVal;

            std::shared_ptr<Servo> servo = sm->findServoById(it->servoId);
            if (servo == nullptr) {
                LOG(WARNING) << "Servo ID:" << it->servoId << " not found.";
            }

            std::shared_ptr<IoController> ioController = cm->findControllerById(servo->getControllerId());
            if (ioController == nullptr) {
                LOG(WARNING) << "Controller ID:" << servo->getControllerId() << " not found.";
            }

            int targetPulse = (it->targetDeg / 90) * 1000 + 1000;
            LOG(INFO) << "Target:" << targetPulse;

            ioController->moveToPulse(*servo, targetPulse);
        }


    }

}


}
