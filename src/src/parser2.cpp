#include "continuation.h"
#include "evaluator.h"
#include "list.h"
#include "parser2.h"
#include "queue.h"
#include "string.h"

namespace plx {

    Parser::Parser()
        : Any{T_Parser} {
        _pos = 0;
        _tokens = new Queue();
    }

    static void _number(Evaluator* etor, Any* arg, Continuation* contin) {
        std::cout << "parser2 _number got here 1\n";
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        std::cout << "parser2 _number got here 2, c = " << c << "\n";
        if (isdigit(c)) {
            std::cout << "parser2 _number got here 3, is a digit\n";
            parser->_lexeme << c;
            etor->pushExpr(contin);
            std::cout << "parser2 _number got here 4, etor = " << etor << "\n";
        }
        else {
            std::cout << "parser2 _number got here 5, c = " << c << "\n";
        }
    }

    static void _symbol(Evaluator* etor, Any* arg, Continuation* contin) {
        std::cout << "parser2 _symbol got here 1\n";
        (void)etor;
        (void)arg;
        (void)contin;
    }

    void Parser::parse(Evaluator* etor) {
        std::cout << "parser2 parse got here 1\n";
        List* args = (List*)etor->popObj();
        std::cout << "parser2 parse got here 2, args = " << args << "\n";
        String* str = (String*)args->_first;
        _inputString = str->_str;
        char c = _inputString[_pos++];
        std::cout << "parser2 parse got here 2, c = " << c << "\n";
        if (isalpha(c)) {
            Continuation* contin = new Continuation("parser:symbol", _symbol, this);
            etor->pushExpr(contin);
        }
        else if (isdigit(c)) {
            Continuation* contin = new Continuation("parser:number", _number, this);
            etor->pushExpr(contin);
        }
    }

    void Parser::show(std::ostream& stream) {
        stream << "A-PARSER";
    }

}
