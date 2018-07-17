#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../data/DatabaseManager.h"
#include "Sequence.h"

#include <boost/thread.hpp>
#include <boost/thread/scoped_thread.hpp>

namespace r7 {

class SequenceManager
{
public:
    SequenceManager(const std::shared_ptr<DatabaseManager> dbm);
    virtual ~SequenceManager() = default;

    std::shared_ptr<Sequence> findById(const std::string& sequenceId) const;

    void play(const std::string& sequenceId, const float speed);

private:
    void processLoop();

private:
    boost::scoped_thread<> processThread;

    const std::shared_ptr<DatabaseManager> dbm;
};

}
