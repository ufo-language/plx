#pragma once

#include "src/any.h"

namespace plx {

    struct Integer : public Any {
        Integer(const int i) : Any{T_Integer}, _i{i} {}
        const int _i;
        void show(std::ostream& stream) override { stream << _i; }
    };

}
