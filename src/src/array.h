#pragma once

#include "src/any.h"

namespace plx {

    struct Array : public Any {
        Array(int count, Any** elems);
        int _count;
        Any** _elems;
        void show(std::ostream& stream) override;
    };

}
