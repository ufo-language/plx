#include "any.h"
#include "array.h"
#include "evaluator.h"
#include "identifier.h"
#include "triple.h"

namespace plx {

    void Identifier::evaluate(Evaluator* etor) {
        Any* value = etor->lookup(this);
        if (value == nullptr) {
            Any** elems = new Any*[2]{new Identifier("UnboundIdentifier"), this};
            Array* exnAry = new Array(2, elems);
            etor->_exception = exnAry;
            etor->_status = ES_Exception;
        }
        etor->pushObj(value);
    }

}
