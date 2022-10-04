#pragma once

#include "src/any.h"

namespace plx {

    struct Any;
    struct List;
    struct Symbol;
    struct Triple;

    struct Evaluator : public Any {
        Evaluator();
        List* _ostack;
        Triple* _estack;
        Triple* _env;
        EvaluatorStatus _status;
        Any* _exception;
        Triple* bind(Any* name, Any* value);
        void evaluate(List* exprList);
        void evaluate(int nSteps);
        bool findExceptionHandler();
        Any* lookup(Symbol* name);
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
