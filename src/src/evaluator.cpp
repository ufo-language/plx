#include <cassert>

#include "evaluator.h"
#include "exceptionhandler.h"
#include "identifier.h"
#include "list.h"
#include "primitives.h"
#include "triple.h"

namespace plx {

    Evaluator::Evaluator()
        : Any{T_Evaluator}, _ostack{new List()}, _estack{new Triple()}, _env{new Triple()} {
    }

    Triple* Evaluator::bind(Any* name, Any* value) {
        _env = new Triple(name, value, _env);
        return _env;
    }

    // This is a top-level function to be called by the REPL
    // TODO eliminate this function eventually
    void Evaluator::evaluate(List* exprList) {
        exprList = exprList->reverse();
        // TODO convert exprList into a DO expression; intermediate results need to be popped off the object stack
        while (!exprList->isEmpty()) {
            Any* expr = exprList->_first;
            pushExpr(expr);
            exprList = (List*)exprList->_rest;
        }
        // evaluate expressions
        _status = ES_Running;
        bool contin = true;
        while (contin) {
            step();
            switch (_status) {
                case ES_Running:
                    // just keep looping
                    break;
                case ES_Blocked:
                    std::cout << "Evaluator::evaluate switch got ES_Blocked; not implemented yet\n";
                    break;
                case ES_Exception:
                    if (!findExceptionHandler()) {
                        contin = false;
                    }
                    break;
                case ES_Terminated:
                    contin = false;
                    break;
            }
        }
    }

#if 0
    EvaluatorStatus Evaluator::evaluate(int nSteps) {
        while (nSteps > 0) {
            nSteps--;
            EvaluatorStatus es = step();
            if (es != ES_Running) {
                std::cout << "Evaluator::evaluate returning es = " << es << "\n";
                return es;
            }
        }
        return ES_Running;
    }
#endif

    bool Evaluator::findExceptionHandler() {
        while (!_estack->isEmpty()) {
            Any* expr = popExpr();
            if (expr->_typeId == T_ExceptionHandler) {
                ExceptionHandler* exnHandler = (ExceptionHandler*)expr;
                Triple* matchRes = exnHandler->match(_exception, _env);
                if (matchRes != nullptr) {
                    pushExpr(exnHandler->_body, matchRes);
                    return true;
                }
            }
        }
        return false;
    }

    Any* Evaluator::lookup(Identifier* name) {
        Triple* binding = _env->locate(name);
        if (binding != nullptr) {
            return binding->_value;
        }
        return nullptr;
    }

    Any* Evaluator::popExpr() {
        if (_estack->isEmpty()) {
            return nullptr;
        }
        Any* expr = _estack->_key;
        _env = (Triple*)_estack->_value;
        _estack = _estack->_next;
        return expr;
    }

    Any* Evaluator::popObj() {
        Any* obj = _ostack->_first;
        _ostack = (List*)_ostack->_rest;
        assert(_ostack != nullptr);
        return obj;
    }

    void Evaluator::pushExpr(Any* expr) {
        assert(expr != nullptr);
        _estack = new Triple(expr, _env, _estack);
    }

    void Evaluator::pushExpr(Any* expr, Triple* env) {
        _estack = new Triple(expr, env, _estack);
    }

    void Evaluator::pushObj(Any* obj) {
        _ostack = new List(obj, _ostack);
    }

    void Evaluator::run() {
        while (_status == ES_Running) {
            step();
        }
    }

    void Evaluator::show(std::ostream& stream) {
        stream << "[Evaluator "
               << _ostack
               << ' ' << _estack
               //<< ' ' << _env
               << " exception=" << _exception
               << ']';
    }

    void Evaluator::step() {
        Any* expr = popExpr();
        if (expr == nullptr) {
            _status = ES_Terminated;
        }
        else {
            if (_showStep) {
                std::cout << "Evaluator::step expr = " << expr << "\n";
            }
            expr->evaluate(this);
        }
    }

}
