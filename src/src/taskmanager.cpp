#include "src/evaluator.h"
#include "src/queue.h"
#include "src/taskmanager.h"

namespace plx {

    TaskManager::TaskManager() {
        _threads = new Queue();
    }

    TaskManager::~TaskManager() {
    }

    void TaskManager::addThread(Evaluator* etor) {
        _threads->enq(etor);
    }

    void TaskManager::runThreads() {
        std::cout << "TaskManager::runThreads called\n";
        // TODO do not pop the thread before running it. That will leave it ungrounded.
    }

}
