#include <iostream>

#include "src/array.h"
#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/symbol.h"

namespace plx {

    struct ApplyException : public std::exception {
        ApplyException(Any* expr, Any* value)
            : expr{expr}, value{value} {}
        Any* expr;
        Any* value;
    };

    static EvaluatorStatus apply(Evaluator* etor, Any* arg) {
        Any* abstr = etor->popObj();
        EvaluatorStatus es = ES_Exception;
        switch (abstr->_typeId) {
            case T_Prim:
                std::cout << "list.cpp apply got Prim\n";
                break;
            case T_PrimMacro:
                std::cout << "list.cpp apply got PrimMacro\n";
                break;
            default:
                break;
        }
        Symbol* sym = new Symbol("ApplyException");
        Any* aryArgs[] = {sym, abstr, arg};
        etor->_exception = new Array(3, aryArgs);
        es = ES_Exception;
        return es;
    }

    EvaluatorStatus List::evaluate(Evaluator* etor) {
        if (isEmpty()) {
            etor->pushObj(this);
        }
        else {
            Any* app = new Continuation("list", apply, this);
            etor->pushExpr(app);
            etor->pushExpr(_first);
        }
        return ES_Running;
    }

    List* List::reverse() {
        List* list = this;
        List* reversed = new List();
        while (!list->isEmpty()) {
            reversed = new List(list->_first, reversed);
            list = (List*)list->_rest;
        }
        return reversed;
    }

    void List::show(std::ostream& stream) {
        showWith(stream, "[", "]");
    }

    void List::showWith(std::ostream& stream, const std::string& open, const std::string& close) {
        stream << open;
        List* list = this;
        bool _firstIter = true;
        while (!list->isEmpty()) {
            if (_firstIter) {
                _firstIter = false;
            }
            else {
                stream << ' ';
            }
            stream << list->_first;
            if (list->_rest->_typeId == T_List) {
                list = (List*)list->_rest;
            }
            else {
                stream << list->_rest;
                break;
            }
        }
        stream << close;
    }

}
