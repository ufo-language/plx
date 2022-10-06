#include "any.h"
#include "continuation.h"
#include "evaluator.h"
#include "function.h"
#include "identifier.h"
#include "list.h"
#include "nil.h"
#include "primitive.h"
#include "queue.h"
#include "triple.h"

namespace plx {

    static void prim_disp(Evaluator* etor);
    static void prim_do(Evaluator* etor);
    static void prim_fun(Evaluator* etor);
    static void prim_if(Evaluator* etor);
    static void prim_let(Evaluator* etor);
    static void prim_quote(Evaluator* etor);
    static void prim_show(Evaluator* etor);

    static void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_Prim, primFun);
        etor->bind(new Identifier(name), prim);
    }
    
    static void defineMacro(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_PrimMacro, primFun);
        etor->bind(new Identifier(name), prim);
    }
    
    void prim_defineAll(Evaluator* etor) {
        definePrim("disp", prim_disp, etor);
        defineMacro("do", prim_do, etor);
        defineMacro("fun", prim_fun, etor);
        defineMacro("if", prim_if, etor);
        defineMacro("let", prim_let, etor);
        defineMacro("quote", prim_quote, etor);
        definePrim("show", prim_show, etor);
    }

    // primitives follow ---------------------------------------------

    static void prim_disp(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->display(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
    }

    void doContin(Evaluator* etor, Any* arg, Continuation* contin) {
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
                contin->_arg = rest;
                etor->pushExpr(contin);
                etor->pushExpr(first);
            }
        }
    }

    static void prim_do(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        Continuation* contin = new Continuation("do", doContin, args);
        etor->pushExpr(contin);
        etor->pushObj(new Nil());
    }

    static void prim_fun(Evaluator* etor) {
        List* parts = (List*)etor->popObj();
        Any* firstPart = parts->_first;
        Identifier* name = nullptr;
        Triple* envTriple = etor->_env;
        if (firstPart->_typeId == T_Identifier) {
            name = (Identifier*)firstPart;
            envTriple = etor->bind(name, new Nil());
            parts = (List*)parts->_rest;
        }
        List* params = (List*)parts->_first;
        List* body = (List*)parts->_rest;
        Function* fun = new Function(params, body, envTriple);
        if (name != nullptr) {
            envTriple->_value = fun;
        }
        etor->pushObj(fun);
    }

    static void _ifContin(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        List* parts = (List*)arg;
        Any* value = etor->popObj();
        etor->pushObj(
            value->boolValue()
            ? parts->_first
            : ((List*)parts->_rest)->_first
        );
    }

    static void prim_if(Evaluator* etor) {
        List* parts = (List*)etor->popObj();
        Continuation* contin = new Continuation("if", _ifContin, parts->_rest);
        etor->pushExpr(contin);
        etor->pushExpr(parts->_first);
    }

    static void _letContin(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Queue* nameQueue = (Queue*)arg;
        int nNames = nameQueue->_count;
        List* nameList = (List*)nameQueue->_first;
        while (nNames-- > 0) {
            Identifier* name = (Identifier*)nameList->_first;
            Any* value = etor->popObj();
            etor->bind(name, value);
            nameList = (List*)nameList->_rest;
        }
        etor->pushObj(new Nil());
    }

    static void prim_let(Evaluator* etor) {
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
    }

    static void prim_quote(Evaluator* etor) {
        (void)etor;
        // do nothing; leave the list of arguments on the object stack
    }

    static void prim_show(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->show(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
    }

}
