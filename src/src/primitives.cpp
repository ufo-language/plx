#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/nil.h"
#include "src/primitive.h"
#include "src/queue.h"
#include "src/symbol.h"

namespace plx {

    static EvaluatorStatus prim_disp(Evaluator* etor);
    static EvaluatorStatus prim_do(Evaluator* etor);
    static EvaluatorStatus prim_let(Evaluator* etor);
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
        defineMacro("do", prim_do, etor);
        defineMacro("let", prim_let, etor);
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

    static EvaluatorStatus _doContin(Evaluator* etor, Any* arg) {
        etor->popObj();
        List* args = (List*)arg;
        if (args->isEmpty()) {
            etor->pushObj(new Nil());
        }
        else {
            Any* first = args->_first;
            List* rest = (List*)args->_rest;
            if (rest->isEmpty()) {
                etor->pushExpr(first);
            }
            else {
                Continuation* contin = new Continuation("do", _doContin, rest);
                etor->pushExpr(contin);
                etor->pushExpr(first);
            }
        }
        return ES_Running;
    }

    static EvaluatorStatus prim_do(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        Continuation* contin = new Continuation("do", _doContin, args);
        etor->pushExpr(contin);
        etor->pushObj(new Nil());
        return ES_Running;
    }

    static EvaluatorStatus _letContin(Evaluator* etor, Any* arg) {
        Queue* nameQueue = (Queue*)arg;
        int nNames = nameQueue->_count;
        List* nameList = (List*)nameQueue->_first;
        while (nNames-- > 0) {
            Symbol* name = (Symbol*)nameList->_first;
            Any* value = etor->popObj();
            etor->bind(name, value);
            nameList = (List*)nameList->_rest;
        }
        etor->pushObj(new Nil());
        return ES_Running;
    }

    static EvaluatorStatus prim_let(Evaluator* etor) {
        Queue* nameQ = new Queue();
        Continuation* contin = new Continuation("let", _letContin, nameQ);
        etor->pushExpr(contin);
        List* bindings = (List*)etor->popObj();
        while (!bindings->isEmpty()) {
            List* binding = (List*)bindings->_first;
            Any* name = binding->_first;
            nameQ->enq(name);
            etor->pushExpr(((List*)binding->_rest)->_first);
            bindings = (List*)bindings->_rest;
        }
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
