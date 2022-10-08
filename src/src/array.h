#pragma once

#include "any.h"

namespace plx {

    struct Evaluator;
    struct Queue;
    struct Triple;

    struct Array : public Any {
        static Array* fromQueue(Queue* q);
        Array(int count);
        Array(int count, Any** elems);
        int _count;
        Any** _elems;
        bool boolValue() { return _count > 0; }
        void evaluate(Evaluator* etor) override;
        Triple* match(Any* other, Triple* env) override;
        void show(std::ostream& stream) override;
    };

}
