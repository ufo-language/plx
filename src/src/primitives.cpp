#include "src/evaluator.h"
#include "src/primitive.h"
#include "src/symbol.h"

namespace plx {

    static EvaluatorStatus prim_print(Evaluator* etor);

    static void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_Prim, primFun);
        etor->bind(new Symbol(name), prim);
    }
    
    static void defineMacro(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_PrimMacro, primFun);
        etor->bind(new Symbol(name), prim);
    }
    
    void prim_defineAll(Evaluator* etor) {
        definePrim("print", prim_print, etor);
    }

    static EvaluatorStatus prim_print(Evaluator* etor) {
        (void)etor;
        return ES_Running;
    }

}
