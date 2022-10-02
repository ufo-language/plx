#pragma once

#include "src/any.h"

namespace plx {

    struct Evaluator;
    struct List;
    struct Triple;

    struct Function : public Any {
        Function(List* params, List* body, Triple* lexEnv)
            : Any{T_Function}, _params{params}, _body{body}, _lexEnv{lexEnv} {}
        List* _params;
        List* _body;
        Triple* _lexEnv;
        void show(std::ostream& stream) override;
    };

}
