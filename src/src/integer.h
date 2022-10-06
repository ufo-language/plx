#pragma once

#include "src/any.h"

namespace plx {

    struct Integer : public Any {
        Integer(const int i) : Any{T_Integer}, _value{i} {}
        const int _value;
        bool boolValue() override { return _value != 0; }
        bool isEqual(Any* other) override { return _value == ((Integer*)other)->_value; }
        void show(std::ostream& stream) override { stream << _value; }
    };

}
