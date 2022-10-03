#include "src/array.h"
#include "src/evaluator.h"
#include "src/symbol.h"

namespace plx {

    void Symbol::evaluate(Evaluator* etor) {
        Any* value = etor->lookup(this);
        if (value == nullptr) {
            Any** elems = new Any*[2]{new Symbol("UnboundIdentifier"), this};
            Array* exnAry = new Array(2, elems);
            etor->_exception = exnAry;
            etor->_status = ES_Exception;
        }
        etor->pushObj(value);
    }

    bool Symbol::isEqual(Any* other) {
        Symbol* otherSym = (Symbol*)other;
        return _name == otherSym->_name;
    }

}
