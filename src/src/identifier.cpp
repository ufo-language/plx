#include "any.h"
#include "array.h"
#include "evaluator.h"
#include "identifier.h"
#include "string.h"
#include "symbol.h"
#include "triple.h"

namespace plx {

    void Identifier::evaluate(Evaluator* etor) {
        Any* value = etor->lookup(this);
        if (value == nullptr) {
            Any** elems = new Any*[3]{new Symbol("Identifier"),
                                      new String("Unbound identifier"),
                                      this};
            Array* exnAry = new Array(3, elems);
            etor->_exception = exnAry;
            etor->_status = ES_Exception;
        }
        etor->pushObj(value);
    }

}
