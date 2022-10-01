#pragma once

#include "src/any.h"

namespace plx {

    struct Boolean : public Any {
        Boolean(const bool b) : Any{T_Nil}, _b{b} {}
        const bool _b;
        void show(std::ostream& stream) override { stream << (_b ? "true" : "false"); }
    };

}
