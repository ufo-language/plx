#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/list.h"

namespace plx {

    EvaluatorStatus Continuation::evaluate(Evaluator* etor) {
        return _continFun(etor, _arg);
    }

    void Continuation::show(std::ostream& stream) {
        stream << "Continuation{" << _name << ", args=" << _arg << "}";
    }

}
