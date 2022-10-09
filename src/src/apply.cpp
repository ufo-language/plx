#include "any.h"
#include "apply.h"
#include "array.h"
#include "continuation.h"
#include "evaluator.h"
#include "function.h"
#include "list.h"
#include "nil.h"
#include "primitive.h"
#include "string.h"
#include "symbol.h"
#include "triple.h"

#include <cassert>

namespace plx {

    void doContin(Evaluator* etor, Any* arg, Continuation* contin);

    void _paramArgMismatchException(Any* fun, Any* param, Any* arg, Evaluator* etor) {
        Symbol* typeSymbol = new Symbol("Apply");
        String* reasonString = new String("parameter/argument mismatch");
        Any** elems = new Any*[5]{typeSymbol, reasonString, fun, param, arg};
        Array* exnAry = new Array(5, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
    }

    void _tooManyArgsException(Any* fun, List* args, Evaluator* etor) {
        Symbol* typeSymbol = new Symbol("Apply");
        String* reasonString = new String("too many arguments");
        Any** elems = new Any*[4]{typeSymbol, reasonString, fun, args};
        Array* exnAry = new Array(4, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
    }

    void _applyFunction(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Function* fun = (Function*)arg;
        List* allArgs = (List*)etor->popObj();
#if 0
        List* args = allArgs;
        List* params = fun->_params;
        Triple* env = fun->_lexEnv;
        // this whole while loop is just to bind args to params
        while (true) {
            if (args->isEmpty()) {
                if (params->isEmpty()) {
                    // all params & args match
                    break;
                }
                // return a curried function
                Function* fun1 = new Function(params, fun->_body, env);
                etor->pushObj(fun1);
                return;
            }
            if (params->isEmpty()) {
                _tooManyArgsException(fun, allArgs, etor);
                return;
            }
            Any* param = params->_first;
            Any* arg = args->_first;
            env = param->match(arg, env);
            if (env == nullptr) {
                _paramArgMismatchException(fun, param, arg, etor);
                return;
            }
            params = (List*)params->_rest;
            assert(params != nullptr);
            args = (List*)args->_rest;
        }
        // evaluate the function body
        Continuation* contin1 = new Continuation("apply", doContin, fun->_body);
        etor->pushExpr(contin1, env);
        etor->pushObj(new Nil());
#else
        fun->apply(allArgs, etor);
#endif
    }

    void _applyPrim(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Primitive* prim = (Primitive*)arg;
        PrimFun primFun = prim->_primFun;
        primFun(etor);
    }

    void _apply(Evaluator *etor, Any* arg, Continuation* contin) {
        (void)contin;
        Any* abstr = etor->popObj();
        TypeId typeId = abstr->_typeId;
        if (typeId == T_Prim) {
            Continuation* continPrim = new Continuation("applyPrim", _applyPrim, abstr);
            etor->pushExpr(continPrim);
            etor->pushExpr(arg);
        }
        else if (typeId == T_PrimMacro) {
            etor->pushObj(arg);
            ((Primitive*)abstr)->_primFun(etor);
        }
        else if (typeId == T_Function) {
            Continuation* continFunction = new Continuation("applyFunction", _applyFunction, abstr);
            etor->pushExpr(continFunction);
            etor->pushExpr(arg);
        }
        else {
            Any** elems = new Any*[2]{new Symbol("ObjectNotApplyable"), abstr};
            Array* exnAry = new Array(2, elems);
            etor->_exception = exnAry;
            etor->_status = ES_Exception;
        }
    }

    void Apply::evaluate(Evaluator* etor) {
        Continuation* contin = new Continuation("apply", _apply, _rest);
        etor->pushExpr(contin);
        etor->pushExpr(_first);
    }

    void Apply::show(std::ostream& stream) {
        this->showWith(stream, START_CHAR, STOP_CHAR);
    }

}
