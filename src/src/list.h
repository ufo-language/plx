#pragma once

#include "src/any.h"

namespace plx {

    struct List : public Any {
        List() : Any{T_List}, _first{nullptr}, _rest{nullptr} {}
        List(Any* first) : Any{T_List}, _first{first}, _rest{new List()} {}
        List(Any* first, Any* rest) : Any{T_List}, _first{first}, _rest{rest} {}
        Any* _first;
        Any* _rest;
        EvaluatorStatus evaluate(Evaluator* etor);
        bool isEmpty() { return _first == nullptr; }
        List* reverse();
        void show(std::ostream& stream) override;
        void showWith(std::ostream& stream, const std::string& open, const std::string& close);
    };

}
