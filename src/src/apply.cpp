#include "src/apply.h"
#include "src/array.h"
#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/primitive.h"
#include "src/symbol.h"

namespace plx {

    EvaluatorStatus _apply2(Evaluator* etor, Any* arg) {
        Primitive* prim = (Primitive*)arg;
        PrimFun primFun = prim->_primFun;
        return primFun(etor);
    }

    EvaluatorStatus _apply(Evaluator *etor, Any* arg) {
        Any* abstr = etor->popObj();
        EvaluatorStatus es = ES_Running;
        switch (abstr->_typeId) {
            case T_Prim: {
                    Continuation* contin = new Continuation("apply2", _apply2, abstr);
                    etor->pushExpr(contin);
                    etor->pushExpr(arg);
                }
                break;
            case T_PrimMacro:
                etor->pushObj(arg);
                ((Primitive*)abstr)->_primFun(etor);
                break;
            default: {
                    Any** elems = new Any*[2]{new Symbol("ObjectNotApplyable"), abstr};
                    Array* exnAry = new Array(2, elems);
                    etor->_exception = exnAry;
                    es = ES_Exception;
                }
                break;
        }
        return es;
    }

    EvaluatorStatus Apply::evaluate(Evaluator* etor) {
        std::cout << "Apply::evaluate this = " << this << "\n";
        Continuation* contin = new Continuation("apply1", _apply, _args);
        etor->pushExpr(contin);
        etor->pushExpr(_abstr);
        return ES_Running;
    }

    void Apply::show(std::ostream& stream) {
        stream << '.' << _abstr;
        _args->showWith(stream, " ", ";");
    }

}
