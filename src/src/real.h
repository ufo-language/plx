#pragma once

#include "src/any.h"

namespace plx {

    struct Real : public Any {
        Real(const double r) : Any{T_Real}, _value{r} {}
        const double _value;
        bool boolValue() override { return _value != 0.0; }
        void show(std::ostream& stream) override { stream << _value; }
    };

}
