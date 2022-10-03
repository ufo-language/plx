#pragma once

namespace plx {

    struct ExceptionHandler : public Any {
        ExceptionHandler(Any* pattern, Any* body) : Any{T_ExceptionHandler}, _pattern{pattern}, _body{body} {}
        Any* _pattern;
        Any* _body;
        void evaluate(Evaluator* etor) { (void)etor; }
        void show(std::ostream& stream);
    };

}
