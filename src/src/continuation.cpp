#include "continuation.h"
#include "evaluator.h"
#include "list.h"

namespace plx {

    void Continuation::evaluate(Evaluator* etor) {
        _continFun(etor, _arg, this);
    }

    void Continuation::show(std::ostream& stream) {
        stream << "Continuation{" << _name << ", args=" << _arg << "}";
    }

}
