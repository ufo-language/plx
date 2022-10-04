#include "apply.h"
#include "array.h"
#include "continuation.h"
#include "evaluator.h"
#include "integer.h"
#include "list.h"
#include "nil.h"
#include "parser.h"
#include "queue.h"
#include "string.h"
#include "symbol.h"
#include "triple.h"

namespace plx {

    Parser::Parser()
        : Any{T_Parser} {
    }

    static void _parse(Evaluator* etor, Any* arg, Continuation* contin);

    void Parser::parse(const std::string& inputString, Evaluator* etor) {
        String* string = new String(inputString);
        parse(string, etor);
    }

    void Parser::parse(String* inputString, Evaluator* etor) {
        List* args = new List(inputString);
        etor->pushObj(args);
        parse(etor);
    }

    void Parser::parse(Evaluator* etor) {
        _tokens = new Queue();
        _queueStack = new List();
        _expectedClose = new Triple();
        _pos = 0;
        etor->_status = ES_Running;
        List* args = (List*)etor->popObj();
        String* str = (String*)args->_first;
        _inputString = str->_str;
        Continuation* contin = new Continuation("parser:parse", _parse, this);
        etor->pushExpr(contin);
    }

    // NB These functions must be plain functions and not C++ member functions
    // because they're stored in Continuation instances.

    static void _addToken(Parser* parser, Any* token) {
        parser->_tokens->enq(token);
        parser->_lexeme.str(std::string());
    }

    static void _makeApplication(Evaluator* etor, Parser* parser, Continuation* contin) {
        Triple* closeTriple = parser->_expectedClose;
        Symbol* exceptionSymbol = nullptr;
        if (closeTriple->isEmpty()) {
            exceptionSymbol = new Symbol("ClosingParenhesisNotExcpected");
            goto EXCEPTION;
        }
        else {
            Integer* expectedCloseChar = (Integer*)closeTriple->_key;
            if (expectedCloseChar->_value != ')') {
                exceptionSymbol = new Symbol("ClosingParenthesisExcpected");
                goto EXCEPTION;
            }
            List* tokens = parser->_tokens->_first;
            Any* appObj;
            if (tokens->isEmpty()) {
                appObj = new Nil();
            }
            else {
                Any* abstrObj = tokens->_first;
                Any* restObj = tokens->_rest;
                if (restObj->_typeId != T_List) {
                    exceptionSymbol = new Symbol("ProperListExpected");
                    goto EXCEPTION;
                }
                List* args = (List*)restObj;
                appObj = new Apply(abstrObj, args);
            }
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(appObj);
            etor->pushExpr(contin);
        }
        return;
    EXCEPTION:
        Any** elems = new Any*[3]{exceptionSymbol,
                                  closeTriple->_key,
                                  closeTriple->_value};
        Array* exnAry = new Array(3, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
    }

    static void _makeArray(Evaluator* etor, Parser* parser, Continuation* contin) {
        Triple* closeTriple = parser->_expectedClose;
        Symbol* exceptionSymbol = nullptr;
        if (closeTriple->isEmpty()) {
            exceptionSymbol = new Symbol("ClosingBraceNotExcpected");
            goto EXCEPTION;
        }
        else {
            Integer* expectedCloseChar = (Integer*)closeTriple->_key;
            if (expectedCloseChar->_value != '}') {
                exceptionSymbol = new Symbol("ClosingBraceExcpected");
                goto EXCEPTION;
            }
            Array* ary = Array::fromQueue(parser->_tokens);
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(ary);
            etor->pushExpr(contin);
        }
        return;
    EXCEPTION:
        Any** elems = new Any*[3]{exceptionSymbol,
                                  closeTriple->_key,
                                  closeTriple->_value};
        Array* exnAry = new Array(3, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
    }

    static void _makeList(Evaluator* etor, Parser* parser, Continuation* contin) {
        Triple* closeTriple = parser->_expectedClose;
        Symbol* exceptionSymbol = nullptr;
        if (closeTriple->isEmpty()) {
            exceptionSymbol = new Symbol("ClosingBracketNotExpected");
            goto EXCEPTION;
        }
        else {
            Integer* expectedCloseChar = (Integer*)closeTriple->_key;
            if (expectedCloseChar->_value != ']') {
                exceptionSymbol = new Symbol("ClosingBracketExpected");
                goto EXCEPTION;
            }
            List* list = parser->_tokens->_first;
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(list);
            etor->pushExpr(contin);
        }
        return;
    EXCEPTION:
        Any** elems = new Any*[3]{exceptionSymbol,
                                  closeTriple->_key,
                                  closeTriple->_value};
        Array* exnAry = new Array(3, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
    }

    static void _parseNumber(Evaluator* etor, Any* arg, Continuation* contin) {
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

    static void _parseString(Evaluator* etor, Any* arg, Continuation* contin) {
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

    static void _parseSymbol(Evaluator* etor, Any* arg, Continuation* contin) {
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
            contin->_continFun = _parseSymbol;
            etor->pushExpr(contin);
        }
        else if (isdigit(c)) {
            parser->_iValue = c - '0';
            contin->_name = "parser:number";
            contin->_continFun = _parseNumber;
            etor->pushExpr(contin);
        }
        else if (isblank(c)) {
            etor->pushExpr(contin);
        }
        else if (c == '"') {
            contin->_name = "parser:string";
            contin->_continFun = _parseString;
            etor->pushExpr(contin);
        }
        else if (c == '(') {
            parser->_queueStack = new List(parser->_tokens, parser->_queueStack);
            parser->_tokens = new Queue();
            parser->_expectedClose = new Triple(new Integer(')'),
                                                new Integer(parser->_pos),
                                                parser->_expectedClose);
            etor->pushExpr(contin);
        }
        else if (c == ')') {
            _makeApplication(etor, parser, contin);
        }
        else if (c == '[') {
            parser->_queueStack = new List(parser->_tokens, parser->_queueStack);
            parser->_tokens = new Queue();
            parser->_expectedClose = new Triple(new Integer(']'),
                                                new Integer(parser->_pos),
                                                parser->_expectedClose);
            etor->pushExpr(contin);
        }
        else if (c == ']') {
            _makeList(etor, parser, contin);
        }
        else if (c == '{') {
            parser->_queueStack = new List(parser->_tokens, parser->_queueStack);
            parser->_tokens = new Queue();
            parser->_expectedClose = new Triple(new Integer('}'),
                                                new Integer(parser->_pos),
                                                parser->_expectedClose);
            etor->pushExpr(contin);
        }
        else if (c == '}') {
            _makeArray(etor, parser, contin);
        }
        else if (c == '\0') {
            // EOI
            Any* token = parser->_tokens->_first;
            etor->pushObj(token);
        }
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
