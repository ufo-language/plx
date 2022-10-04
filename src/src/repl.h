#pragma once

#include <string>

namespace plx {

    struct Any;
    struct List;
    struct Parser;
    struct Evaluator;

    struct REPL {
        int loop();
        List* _parse(const std::string& line, Parser* parser, Evaluator* etor);
        Any* _eval(List* exprList, Evaluator* etor);
    };

}
