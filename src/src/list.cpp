#include <iostream>

#include "array.h"
#include "continuation.h"
#include "evaluator.h"
#include "list.h"
#include "nil.h"
#include "primitive.h"
#include "symbol.h"

namespace plx {

    struct ApplyException : public std::exception {
        ApplyException(Any* expr, Any* value)
            : expr{expr}, value{value} {}
        Any* expr;
        Any* value;
    };

    static void _makeList(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)arg;
        (void)contin;
        Any* first = etor->popObj();
        Any* rest = etor->popObj();
        List* list = new List(first, rest);
        etor->pushObj(list);
    }

    void List::evaluate(Evaluator* etor) {
        if (isEmpty()) {
            etor->pushObj(this);
        }
        else {
            Continuation* contin = new Continuation("list", _makeList, new Nil());
            etor->pushExpr(contin);
            etor->pushExpr(_first);
            etor->pushExpr(_rest);
        }
    }

    bool List::isEqual(Any* other) {
        List* otherList = (List*)other;
        return Any::IsEqual(_first, otherList->_first) && Any::IsEqual(_rest, otherList->_rest);
    }

    Triple* List::match(Any* other, Triple* env) {
        List* otherList = (List*)other;
        env = Any::Match(_first, otherList->_first, env);
        if (env == nullptr) {
            return nullptr;
        }
        return Any::Match(_rest, otherList->_rest, env);
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
        showWith(stream, START_CHAR, STOP_CHAR);
    }

    void List::showElems(std::ostream& stream) {
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
                stream << " | " << list->_rest;
                break;
            }
        }
    }

    void List::showWith(std::ostream& stream, char open, char close) {
        stream << open;
        showElems(stream);
        stream << close;
    }

}
