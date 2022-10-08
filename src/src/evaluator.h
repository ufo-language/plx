#pragma once

#include "any.h"

namespace plx {

    struct Any;
    struct Identifier;
    struct List;
    struct Triple;

    struct Evaluator : public Any {
        Evaluator();
        List* _ostack;
        Triple* _estack;
        Triple* _env;
        EvaluatorStatus _status = ES_Running;
        Any* _exception = nullptr;
        bool _showStep = false;
        Triple* bind(Any* name, Any* value);
        void evaluate(List* exprList);
        void evaluate(int nSteps);
        bool findExceptionHandler();
        Any* lookup(Identifier* name);
        Any* popExpr();
        Any* popObj();
        void pushExpr(Any* expr);
        void pushExpr(Any* expr, Triple* env);
        void pushObj(Any* obj);
        void run();
        void show(std::ostream& stream);
        void step();
    };

}
