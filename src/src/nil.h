#pragma once

#include "src/any.h"

namespace plx {

    struct Nil : public Any {
        Nil() : Any{T_Nil} {}
        bool boolValue() override { return false; }
        void show(std::ostream& stream) override { stream << "nil"; }
    };

}
