#pragma once

#include "any.h"

namespace plx {

    struct Evaluator;
    struct List;
    struct Triple;

    struct Function : public Any {
        Function(List* params, List* body, Triple* lexEnv)
            : Any{T_Function}, _params{params}, _body{body}, _lexEnv{lexEnv}, _nextRule{nullptr} {}
        List* _params;
        List* _body;
        Triple* _lexEnv;
        Function* _nextRule;
        void apply(List* args, Evaluator* etor);
        void show(std::ostream& stream) override;
    };

}
