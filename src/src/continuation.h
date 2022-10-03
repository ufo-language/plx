#pragma once

#include "src/any.h"

namespace plx {

    struct List;

    typedef void (*ContinFun)(Evaluator *etor, Any* arg);

    struct Continuation : public Any {
        Continuation(const std::string& name, ContinFun continFun, Any* arg)
            : Any{T_Continuation}, _name{name}, _continFun{continFun}, _arg{arg} {}
        std::string _name;
        ContinFun _continFun;
        Any* _arg;
        void evaluate(Evaluator* etor);
        void show(std::ostream& stream);
    };

}
