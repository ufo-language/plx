#pragma once

#include "src/any.h"

namespace plx {

    struct List;
    
    struct Apply : public Any {
        Apply(Any* abstr, List* args) : Any{T_Apply}, _abstr{abstr}, _args{args} {}
        Any* _abstr;
        List* _args;
        EvaluatorStatus evaluate(Evaluator* etor) override;
        void show(std::ostream& stream) override;
    };

}
