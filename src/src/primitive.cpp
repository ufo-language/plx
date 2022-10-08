#include "evaluator.h"
#include "identifier.h"
#include "primitive.h"

namespace plx {

    void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_Prim, primFun);
        etor->bind(new Identifier(name), prim);
    }
    
    void defineMacro(const std::string& name, PrimFun primFun, Evaluator* etor) {
        Primitive* prim = new Primitive(name, T_PrimMacro, primFun);
        etor->bind(new Identifier(name), prim);
    }
    
    void Primitive::show(std::ostream& stream) {
        stream << _name << '@' << (void*)_primFun;
    }

}
