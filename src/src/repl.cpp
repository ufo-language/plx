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
    // TODO or rewrite this using continuations? Then put it in a .so file.

    List* REPL::_parse(const std::string& line, Parser* parser, Evaluator* etor) {
        parser->parse(line, etor);
        if (etor->_status == ES_Running) {
            etor->run();
        }
        List* exprList = nullptr;
        switch (etor->_status) {
            case ES_Running:
                // should not happen
            case ES_Blocked:
                std::cout << "REPL::loop switch got ES_Blocked or ES_Running; not implemented yet\n";
                break;
            case ES_Exception:
                std::cout << "PARSE ERROR: " << etor->_exception << "\n";
                break;
            case ES_Terminated:
                exprList = (List*)etor->popObj();
                std::cout << "parse exprs = " << exprList << '\n';
                break;
        }
        return exprList;
    }

    int REPL::loop(void) {
        Evaluator* etor = new Evaluator();
        Parser* parser = new Parser();
        prim_defineAll(etor);
        while (true) {
            printf("plx> ");
            std::string line;
            if (std::getline(std::cin, line)) {
                std::cout << "input string = '" << line << "'\n";
                if (line.length() > 0) {
                    List* exprList = _parse(line, parser, etor);
                    std::cout << "parse exprs = " << exprList << '\n';
                    etor->evaluate(exprList);
                    Any* res = etor->popObj();
                    std::cout << res << '\n';
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
