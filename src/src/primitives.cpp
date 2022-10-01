#include "src/evaluator.h"
#include "src/list.h"
#include "src/nil.h"
#include "src/primitive.h"
#include "src/symbol.h"

namespace plx {

    static EvaluatorStatus prim_disp(Evaluator* etor);
    static EvaluatorStatus prim_quote(Evaluator* etor);
    static EvaluatorStatus prim_show(Evaluator* etor);

    static void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_Prim, primFun);
        etor->bind(new Symbol(name), prim);
    }
    
    static void defineMacro(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_PrimMacro, primFun);
        etor->bind(new Symbol(name), prim);
    }
    
    void prim_defineAll(Evaluator* etor) {
        definePrim("disp", prim_disp, etor);
        defineMacro("quote", prim_quote, etor);
        definePrim("show", prim_show, etor);
    }

    static EvaluatorStatus prim_disp(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->display(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
        return ES_Running;
    }

    static EvaluatorStatus prim_quote(Evaluator* etor) {
        (void)etor;
        // do nothing; leave the list of arguments on the object stack
        return ES_Running;
    }

    static EvaluatorStatus prim_show(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->show(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
        return ES_Running;
    }

}
