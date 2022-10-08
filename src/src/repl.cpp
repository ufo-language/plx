#include <cstdio>
#include <iostream>
#include <sstream>

#include "any.h"
#include "array.h"
#include "evaluator.h"
#include "list.h"
#include "parser.h"
#include "primitives.h"
#include "repl.h"

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
                std::cout << "Error: " << etor->_exception << "\n";
                break;
            case ES_Terminated:
                exprList = (List*)etor->popObj();
                break;
        }
        return exprList;
    }

    Any* REPL::_eval(List* exprList, Evaluator* etor) {
        etor->evaluate(exprList);
        Any* res = nullptr;
        switch (etor->_status) {
            case ES_Running:
            case ES_Blocked:
                std::cerr << "REPL::loop switch got ES_Blocked or ES_Running; not implemented yet\n";
                break;
            case ES_Exception:
                std::cerr << "Error: " << etor->_exception << "\n";
                break;
            case ES_Terminated:
                res = etor->popObj();
                break;
        }
        return res;
    }

    int REPL::loop(void) {
        Evaluator* etor = new Evaluator();
        Parser* parser = new Parser();
        prim_defineAll(etor);
        while (true) {
            printf("plx> ");
            std::string line;
            if (std::getline(std::cin, line)) {
                if (line.length() > 0) {
                    List* exprList = _parse(line, parser, etor);
                    if (exprList != nullptr) {
                        Any* res = _eval(exprList, etor);
                        if (res != nullptr && res->_typeId != T_Nil) {
                            std::cout << res << '\n';
                        }
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
