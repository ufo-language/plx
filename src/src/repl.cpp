#include <cstdio>
#include <iostream>
#include <sstream>

#include "src/any.h"
#include "src/array.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/parser.h"
#include "src/primitives.h"
#include "src/repl.h"

namespace plx {

    struct Evaluator;

    // TODO don't spend a lot of time on this
    // TODO replace this with a REP-loop written in the source language

    int REPL::loop(void) {
        Evaluator* etor = new Evaluator();
        prim_defineAll(etor);
        while (true) {
            printf("plx> ");
            std::string line;
            if (std::getline(std::cin, line)) {
                std::cout << "input string = '" << line << "'\n";
                if (line.length() > 0) {
                    List* exprList = parse(line);
                    std::cout << "parse exprs = " << exprList << '\n';
                    EvaluatorStatus es = etor->evaluate(exprList);
                    switch (es) {
                        case ES_Running:
                        case ES_Blocked:
                            std::cout << "REPL::loop switch got ES_Blocked or ES_Running; not implemented yet\n";
                            break;
                        case ES_Exception:
                            std::cout << "Exception: " << etor->_exception << "\n";
                            break;
                        case ES_Terminated:
                            Any* res = etor->popObj();
                            std::cout << res << '\n';
                            break;
                    }
                }
            }
            if (!std::cin) {
                std::cout << '\n';
                break;
            }
        }
        return 0;
    }

}
