#pragma once

#include "any.h"

namespace plx {

    struct Triple;

    struct List : public Any {
        const static char START_CHAR = '[';
        const static char STOP_CHAR = ']';
        List() : List{T_List, nullptr, nullptr} {}
        List(Any* first) : List{T_List, first, new List()} {}
        List(Any* first, Any* rest) : List{T_List, first, rest} {}
        List(TypeId typeId, Any* first, Any* rest) : Any{typeId}, _first{first}, _rest{rest} {}
        Any* _first;
        Any* _rest;
        bool boolValue() { return !isEmpty(); }
        void evaluate(Evaluator* etor);
        bool isEmpty() { return _first == nullptr; }
        bool isEqual(Any* other) override;
        Triple* match(Any* other, Triple* env) override;
        List* reverse();
        void show(std::ostream& stream) override;
        void showWith(std::ostream& stream, char open, char close);
    };

}
