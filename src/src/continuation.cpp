#include "src/continuation.h"
#include "src/evaluator.h"

namespace plx {

    EvaluatorStatus Continuation::evaluate(Evaluator* etor) {
        (void)etor;
        std::cout << "Continuation::evaluate is incomplete: " << this << "\n";
        std::cout << etor << "\n";
        return _continFun(etor, _arg);
    }

    #include <cassert>
    void Continuation::show(std::ostream& stream) {
        stream << "Continuation{" << _name << ", arg=" << _arg << "}";
    }

}
