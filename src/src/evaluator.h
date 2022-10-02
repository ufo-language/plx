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
        Any* _exception;
        Triple* bind(Any* name, Any* value);
        EvaluatorStatus evaluate(List* exprList);
        EvaluatorStatus evaluate(int nSteps);
        bool findExceptionHandler();
        Any* lookup(Symbol* name);
        Any* popExpr();
        Any* popObj();
        void pushExpr(Any* expr);
        void pushExpr(Any* expr, Triple* env);
        void pushObj(Any* obj);
        void show(std::ostream& stream);
        EvaluatorStatus step();
    };

}
