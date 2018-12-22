#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "../data/DatabaseManager.h"
#include "../servos/ServoManager.h"
#include "../controllers/ControllerManager.h"
#include "Sequence.h"

namespace r7 {

class SequenceManager
{
public:
    SequenceManager(const std::shared_ptr<DatabaseManager> dbm,
                    const std::shared_ptr<ServoManager> sm,
                    const std::shared_ptr<ControllerManager> cm);
    virtual ~SequenceManager();

    std::shared_ptr<Sequence> findById(const std::string& sequenceId) const;

    void play(const std::string& sequenceId, const float speed);

private:
    void processLoop();

private:
    bool running = true;

    const std::shared_ptr<DatabaseManager> dbm;
    const std::shared_ptr<ServoManager> sm;
    const std::shared_ptr<ControllerManager> cm;

    std::queue<std::string> sequenceQueue; // order of declaration affects order of initialization

    std::mutex m;
    std::condition_variable cv;
    std::thread processThread;


};

}
