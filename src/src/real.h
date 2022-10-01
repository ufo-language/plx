#pragma once

#include "src/any.h"

namespace plx {

    struct Real : public Any {
        Real(const double r) : Any{T_Real}, _r{r} {}
        const double _r;
        void show(std::ostream& stream) override { stream << _r; }
    };

}
