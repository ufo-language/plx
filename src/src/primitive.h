#pragma once

#include <iostream>

#include "any.h"

namespace plx {

    struct Evaluator;

    typedef void (*PrimFun)(Evaluator *etor);

    struct Primitive : public Any {
        // The TypeId must be supplied to this constructor, but it's expected
        // to be either T_Prim or T_PrimMacro.
        Primitive(const std::string& name, TypeId typeId, PrimFun primFun)
            : Any{typeId}, _name{name}, _primFun{primFun} {}
        std::string _name;
        PrimFun _primFun;
        void show(std::ostream& stream) override;
    };

    void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor);
    void defineMacro(const std::string& name, PrimFun primFun, Evaluator* etor);

}
