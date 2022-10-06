#pragma once

#include "identifier.h"
#include "triple.h"

namespace plx {

    struct Identifier : public Any {
        Identifier(const std::string& name) : Any{T_Identifier}, _name{name} {}
        const std::string _name;
        void evaluate(Evaluator* etor) override;
        bool isEqual(Any* other) override { return _name == ((Identifier*)other)->_name; }
        Triple* match(Any* other, Triple* env) override { return new Triple(this, other, env); }
        void show(std::ostream& stream) override { stream << _name; }
    };

}
