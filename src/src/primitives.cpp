#include <cstdlib>

#include "any.h"
#include "boolean.h"
#include "continuation.h"
#include "evaluator.h"
#include "function.h"
#include "identifier.h"
#include "list.h"
#include "nil.h"
#include "primitive.h"
#include "queue.h"
#include "string.h"
#include "triple.h"

namespace plx {

    // functions defined in other cpp files
    void prim_dl_open(Evaluator* etor);  // in prim_dl.cpp

    // functions defined in this file
    static void prim_do(Evaluator* etor);
    static void prim_fun(Evaluator* etor);
    static void prim_if(Evaluator* etor);
    static void prim_let(Evaluator* etor);
    static void prim_match(Evaluator* etor);
    static void prim_quote(Evaluator* etor);

    static void prim_io_disp(Evaluator* etor);
    static void prim_io_show(Evaluator* etor);

    static void prim_os_pwd(Evaluator* etor);

    void prim_defineAll(Evaluator* etor) {
        defineMacro("do", prim_do, etor);
        defineMacro("fun", prim_fun, etor);
        defineMacro("if", prim_if, etor);
        defineMacro("let", prim_let, etor);
        defineMacro("match", prim_match, etor);
        defineMacro("quote", prim_quote, etor);

        definePrim("dl:open", prim_dl_open, etor);

        definePrim("io:disp", prim_io_disp, etor);
        definePrim("io:show", prim_io_show, etor);

        definePrim("os:pwd", prim_os_pwd, etor);
    }

    // primitives follow ---------------------------------------------

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
        std::cout << "prim_fun parsts = " << parts << "\n";
        Any* firstPart = parts->_first;
        Identifier* name = nullptr;
        Triple* envTriple = etor->_env;
        if (firstPart->_typeId == T_Identifier) {
            name = (Identifier*)firstPart;
            envTriple = etor->bind(name, new Nil());
            parts = (List*)parts->_rest;
        }
        Function* funHead = nullptr;
        Function* funNext = nullptr;
        while (!parts->isEmpty()) {
            List* part = (List*)parts->_first;
            List* params = (List*)part->_first;
            List* body = (List*)part->_rest;
            Function* fun = new Function(params, body, envTriple);
            if (funHead == nullptr) {
                funHead = funNext = fun;
            }
            else {
                funNext->_nextRule = fun;
                funNext = fun;
            }
            parts = (List*)parts->_rest;
        }
        if (name != nullptr) {
            envTriple->_value = funHead;
        }
        etor->pushObj(funHead);
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

    static void _matchContin(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Any* lhs = arg;
        Any* rhs = etor->popObj();
        Triple* env = Any::Match(lhs, rhs, etor->_env);
        Boolean* res = new Boolean(env != nullptr);
        etor->pushObj(res);
    }

    static void prim_match(Evaluator* etor) {
        List* parts = (List*)etor->popObj();
        Any* lhs = parts->_first;
        Any* rhs = ((List*)parts->_rest)->_first;
        Continuation* contin = new Continuation("match", _matchContin, lhs);
        etor->pushExpr(contin);
        etor->pushExpr(rhs);
    }

    static void prim_quote(Evaluator* etor) {
        (void)etor;
        // do nothing; leave the list of arguments on the object stack
    }

    static void prim_io_disp(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->display(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
    }

    static void prim_io_show(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        while (!args->isEmpty()) {
            args->_first->show(std::cout);
            args = (List*)args->_rest;
        }
        etor->pushObj(new Nil());
    }

    static void prim_os_pwd(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        (void)args;
        char* pwd = realpath(".", NULL);
        String* pwdString = new String(pwd);
        etor->pushObj(pwdString);
    }

}
