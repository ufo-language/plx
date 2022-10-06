#pragma once

#include "src/any.h"

namespace plx {

    struct Identifier : public Any {
        Identifier(const std::string& name) : Any{T_Identifier}, _name{name} {}
        const std::string _name;
        void evaluate(Evaluator* etor) override;
        bool isEqual(Any* other);
        Triple* match(Any* other, Triple* env) override;
        void show(std::ostream& stream) override { stream << _name; }
    };

}
