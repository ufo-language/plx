#pragma once

#include "src/any.h"

namespace plx {

    struct Symbol : public Any {
        Symbol(const std::string& name) : Any{T_Symbol}, _name{name} {}
        const std::string _name;
        void evaluate(Evaluator* etor) override;
        bool isEqual(Any* other);
        Triple* match(Any* other, Triple* env) override;
        void show(std::ostream& stream) override { stream << _name; }
    };

}
