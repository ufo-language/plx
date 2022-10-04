#pragma once

#include "src/any.h"

namespace plx {

    struct Queue;

    struct Array : public Any {
        static Array* fromQueue(Queue* q);
        Array(int count);
        Array(int count, Any** elems);
        int _count;
        Any** _elems;
        bool boolValue() { return _count > 0; }
        // TODO void evaluate(Evaluator* etor) override;
        void show(std::ostream& stream) override;
    };

}
