#pragma once

#include "any.h"

namespace plx {

    struct Symbol : public Any {
        Symbol(const std::string& name) : Any{T_Symbol}, _name{name} {}
        const std::string _name;
        bool isEqual(Any* other) override { return _name == ((Symbol*)other)->_name; }
        void show(std::ostream& stream) override { stream << _name; }
    };

}
