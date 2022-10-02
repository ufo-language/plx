#pragma once

#include "src/any.h"

namespace plx {

    struct Array : public Any {
        Array(int count, Any** elems);
        int _count;
        Any** _elems;
        bool boolValue() { return _count > 0; }
        void show(std::ostream& stream) override;
    };

}
