#pragma once

#include "src/any.h"

namespace plx {

    struct List;

    struct Apply : public Any {
        const static char START_CHAR = '(';
        const static char STOP_CHAR = ')';
        Apply(Any* abstr, List* args) : Any{T_Apply}, _abstr{abstr}, _args{args} {}
        Any* _abstr;
        List* _args;
        void evaluate(Evaluator* etor) override;
        void show(std::ostream& stream) override;
    };

}
