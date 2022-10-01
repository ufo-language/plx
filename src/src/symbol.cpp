#include "src/array.h"
#include "src/evaluator.h"
#include "src/symbol.h"

namespace plx {

    EvaluatorStatus Symbol::evaluate(Evaluator* etor) {
        Any* value = etor->lookup(this);
        if (value == nullptr) {
            Any** elems = new Any*[2]{new Symbol("UnboundIdentifier"), this};
            Array* exnAry = new Array(2, elems);
            etor->_exception = exnAry;
            return ES_Exception;
        }
        etor->pushObj(value);
        return ES_Running;
    }

}
