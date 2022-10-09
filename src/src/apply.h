#pragma once

#include "list.h"

namespace plx {

    struct List;

    struct Apply : public List {
        const static char START_CHAR = '(';
        const static char STOP_CHAR = ')';
        Apply(Any* abstr, List* args) : List{T_Apply, abstr, args} {}
        void evaluate(Evaluator* etor) override;
        void show(std::ostream& stream) override;
    };

}
