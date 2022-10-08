#pragma once

#include "any.h"

namespace plx {

    struct Boolean : public Any {
        Boolean(const bool b) : Any{T_Boolean}, _b{b} {}
        const bool _b;
        bool boolValue() override { return _b; }
        void show(std::ostream& stream) override { stream << (_b ? "true" : "false"); }
    };

}
