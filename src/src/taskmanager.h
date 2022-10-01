#pragma once

namespace plx {

    struct Evaluator;
    struct Queue;

    struct TaskManager {
        TaskManager();
        ~TaskManager();
        Queue* _threads;
        void addThread(Evaluator* etor);
        void runThreads();
    };

}
