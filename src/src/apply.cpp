#include "any.h"
#include "apply.h"
#include "array.h"
#include "continuation.h"
#include "evaluator.h"
#include "function.h"
#include "list.h"
#include "nil.h"
#include "primitive.h"
#include "symbol.h"
#include "triple.h"

namespace plx {

    void doContin(Evaluator* etor, Any* arg, Continuation* contin);

#if 0
    static void _parseException(const std::string& reason, Parser* parser, Evaluator* etor) {
        Triple* triple = parser->_expectedClose;
        Symbol* typeSymbol = new Symbol("Parser");
        String* reasonString = new String(reason);
        Integer* charInt = (Integer*)triple->_key;
        String* expectedCharStr = new String(std::string(1, (char)charInt->_value));
        Integer* startPosInt = (Integer*)triple->_value;
        Integer* currentPostInt = new Integer(parser->_pos);
        List* startPos = new List(new Symbol("StartPos"), startPosInt);
        List* currentPos = new List(new Symbol("CurrentPos"), currentPostInt);
        List* expectedChar = new List(new Symbol("ExpectedChar"), expectedCharStr);
        Any** elems = new Any*[5]{typeSymbol,
                                  reasonString,
                                  startPos,
                                  currentPos,
                                  expectedChar};
        Array* exnAry = new Array(5, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
        parser->_lexeme.str(std::string());
        parser->_pos = 0;
    }
#endif
    
    void _applyException() {
    }

    void _applyFunction(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Function* fun = (Function*)arg;
        List* args = (List*)etor->popObj();
        List* params = fun->_params;
        Triple* env = fun->_lexEnv;
        // bind args to params
        while (true) {
            if (args->isEmpty()) {
                if (!params->isEmpty()) {
                    // return a new function
                    Function* fun1 = new Function(params, fun->_body, env);
                    etor->pushObj(fun1);
                    return;
                }
                else {
                    // all params & args match
                    break;
                }
            }
            if (params->isEmpty()) {
                // too many arguments
            }
            Any* param = params->_first;
            Any* arg = args->_first;
            env = param->match(arg, env);
            if (env == nullptr) {
                // parameter/argument mismatch
            }
            params = (List*)params->_rest;
            args = (List*)args->_rest;
        }
        // evaluate the function body
        Continuation* contin1 = new Continuation("apply", doContin, fun->_body);
        etor->pushExpr(contin1, env);
        etor->pushObj(new Nil());
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
        Continuation* contin = new Continuation("apply", _apply, _args);
        etor->pushExpr(contin);
        etor->pushExpr(_abstr);
    }

    void Apply::show(std::ostream& stream) {
        stream << START_CHAR << _abstr;
        _args->showWith(stream, " ", std::string(1, STOP_CHAR));
    }

}
