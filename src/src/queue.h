#pragma once

#include "src/any.h"
#include "src/list.h"

namespace plx {

    struct Evaluator;
    struct List;

    struct Queue : public Any {
        Queue() : Any{T_Queue}, _first{new List()}, _last{_first}, _count{0} {}
        List* _first;
        List* _last;
        int _count;
        bool boolValue() override { return _count > 0; }
        void enq(Any* elem);
        void deq(Evaluator* etor);
        Any* deq_unsafe();
        void show(std::ostream& stream) override;
    };

}
