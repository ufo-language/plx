#include "continuation.h"
#include "evaluator.h"
#include "integer.h"
#include "list.h"
#include "parser2.h"
#include "queue.h"
#include "string.h"
#include "symbol.h"

namespace plx {

    Parser::Parser()
        : Any{T_Parser} {
        _pos = 0;
        _tokens = new Queue();
    }

    static void _addToken(Parser* parser, Any* token) {
        parser->_tokens->enq(token);
        parser->_lexeme.str(std::string());
    }

    static void _parse(Evaluator* etor, Any* arg, Continuation* contin);

    static void _number(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (isdigit(c)) {
            parser->_iValue = parser->_iValue * 10 + (c - '0');
        }
        else {
            parser->_pos--;
            contin->_continFun = _parse;
            Integer* i = new Integer(parser->_iValue);
            _addToken(parser, i);
        }
        etor->pushExpr(contin);
    }

    static void _string(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (c == '\"') {
            contin->_continFun = _parse;
            String* s = new String(parser->_lexeme.str());
            _addToken(parser, s);
        }
        else {
            parser->_lexeme << c;
        }
        etor->pushExpr(contin);
    }

    static void _symbol(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (isalpha(c)) {
            parser->_lexeme << c;
        }
        else {
            parser->_pos--;
            contin->_continFun = _parse;
            Symbol* s = new Symbol(parser->_lexeme.str());
            _addToken(parser, s);
        }
        etor->pushExpr(contin);
    }

    static void _parse(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (isalpha(c)) {
            parser->_lexeme << c;
            contin->_name = "parser:symbol";  // TODO after this all works, don't bother changing the name of the parser anywhere
            contin->_continFun = _symbol;
            etor->pushExpr(contin);
        }
        else if (isdigit(c)) {
            parser->_iValue = c - '0';
            contin->_name = "parser:number";
            contin->_continFun = _number;
            etor->pushExpr(contin);
        }
        else if (isblank(c)) {
            etor->pushExpr(contin);
        }
        else if (c == '"') {
            // parse string
            contin->_name = "parser:string";
            contin->_continFun = _string;
            etor->pushExpr(contin);
        }
        else if (c == '(') {
            // parse application
        }
        else if (c == '[') {
            // parse list
        }
        else if (c == '{') {
            // parse array
        }
        else if (c == '\0') {
            // got EOI
            std::cout << "parser _parse got EOI, tokens = " << parser->_tokens << "\n";
            etor->pushObj(parser->_tokens->_first);
        }
    }

    void Parser::parse(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        String* str = (String*)args->_first;
        _inputString = str->_str;
        Continuation* contin = new Continuation("parser:parse", _parse, this);
        etor->pushExpr(contin);
    }

    void Parser::show(std::ostream& stream) {
        stream << "Parser{"
               << "inputString='" << _inputString
               << "', pos=" << _pos
               << ", lexeme='" << _lexeme.str()
               << "', tokens=" << _tokens
               << "}";
    }

}
