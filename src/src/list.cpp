#include <iostream>

#include "src/array.h"
#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/nil.h"
#include "src/primitive.h"
#include "src/symbol.h"

namespace plx {

    struct ApplyException : public std::exception {
        ApplyException(Any* expr, Any* value)
            : expr{expr}, value{value} {}
        Any* expr;
        Any* value;
    };

    EvaluatorStatus makeList(Evaluator* etor, Any* arg) {
        (void)arg;
        Any* first = etor->popObj();
        Any* rest = etor->popObj();
        List* list = new List(first, rest);
        etor->pushObj(list);
        return ES_Running;
    }

    EvaluatorStatus List::evaluate(Evaluator* etor) {
        if (isEmpty()) {
            etor->pushObj(this);
        }
        else {
            Continuation* contin = new Continuation("list", makeList, new Nil());
            etor->pushExpr(contin);
            etor->pushExpr(_first);
            etor->pushExpr(_rest);
        }
        return ES_Running;
    }

    bool List::isEqual(Any* other) {
        std::cout << "List::isEqual is incomplete\n";
        (void)other;
        return false;
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
