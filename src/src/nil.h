#pragma once

#include "src/any.h"

namespace plx {

    struct Nil : public Any {
        Nil() : Any{T_Nil} {}
        void show(std::ostream& stream) override { stream << "nil"; }
    };

}
