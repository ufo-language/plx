#include "apply.h"
#include "continuation.h"
#include "evaluator.h"
#include "function.h"
#include "list.h"
#include "nil.h"
#include "triple.h"

namespace plx {

    void doContin(Evaluator* etor, Any* arg, Continuation* contin);

    void Function::apply(List* args, Evaluator* etor) {
        std::cout << "Function::apply got here, args = " << args << "\n";
        Function* fun = this;
        while (fun != nullptr) {
            std::cout << "  checking " << fun << "\n";
            List* params = fun->_params;
            Triple* env = params->match(args, fun->_lexEnv);
            if (env != nullptr) {
                Continuation* contin1 = new Continuation("apply", doContin, fun->_body);
                etor->pushExpr(contin1, env);
                etor->pushObj(new Nil());
                return;
            }
            fun = fun->_nextRule;
        }
    }

    void Function::show(std::ostream& stream) {
        stream << Apply::START_CHAR << "fun ";
        Function* fun = this;
        bool firstIter = true;
        while (fun != nullptr) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                stream << ' ';
            }
            stream << '[' << fun->_params << ' ' << fun->_body << ']';
            fun = fun->_nextRule;
        }
        stream << Apply::STOP_CHAR;
    }

}
