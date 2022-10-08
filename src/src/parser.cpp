#include "apply.h"
#include "array.h"
#include "boolean.h"
#include "continuation.h"
#include "evaluator.h"
#include "identifier.h"
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

    // NB Many of these functions must be plain functions and not C++ member
    // functions because they're stored in Continuation instances.

    static void _addToken(Parser* parser, Any* token) {
        parser->_tokens->enq(token);
        parser->_lexeme.str(std::string());
    }

    static Any* _checkSymbol(const std::string& name) {
        if (name == "true") {
            return new Boolean(true);
        }
        if (name == "false") {
            return new Boolean(false);
        }
        if (name == "nil") {
            return new Nil();
        }
        if (name[0] >= 'A' && name[0] <= 'Z') {
            return new Symbol(name);
        }
        return new Identifier(name);    
    }

    static bool _isSpecialChar(char c) {
        static char specialChars[] = {'(', ')', '[', ']', '{', '}', '"', 0};
        char* sc = specialChars;
        do {
            if (c == *sc) {
                return true;
            }
        } while (*sc++);
        return false;
    }

    static void _parseException(const std::string& reason, Parser* parser, Evaluator* etor) {
        Triple* triple = parser->_expectedClose;
        Symbol* typeSymbol = new Symbol("Parser");
        String* reasonString = new String(reason);
        Integer* charInt = (Integer*)triple->_key;
        String* expectedCharStr = new String(std::string(1, (char)charInt->_value));
        Integer* startPosInt = (Integer*)triple->_value;
        Integer* currentPostInt = new Integer(parser->_pos);
        List* startPos = new List(new Symbol("StartPos"), startPosInt);
        List* currentPos = new List(new Symbol("CurrentPos"), currentPostInt);
        List* expectedChar = new List(new Symbol("ExpectedChar"), expectedCharStr);
        Any** elems = new Any*[5]{typeSymbol,
                                  reasonString,
                                  startPos,
                                  currentPos,
                                  expectedChar};
        Array* exnAry = new Array(5, elems);
        etor->_exception = exnAry;
        etor->_status = ES_Exception;
        parser->_lexeme.str(std::string());
        parser->_pos = 0;
    }

    static bool _checkClose(Evaluator* etor, Parser* parser, char expectedChar) {
        Triple* closeTriple = parser->_expectedClose;
        if (closeTriple->isEmpty()) {
            parser->_expectedClose = new Triple(new Integer(expectedChar),
                                                new Integer(parser->_pos),
                                                parser->_expectedClose);
            _parseException("Closing character not expected", parser, etor);
            return false;
        }
        Integer* pushedCloseChar = (Integer*)closeTriple->_key;
        if (pushedCloseChar->_value != (int)expectedChar) {
            _parseException("Closing character expected", parser, etor);
            return false;
        }
        parser->_expectedClose = closeTriple->_next;
        return true;
    }

    static void _makeApplication(Evaluator* etor, Parser* parser, Continuation* contin) {
        if (_checkClose(etor, parser, ')')) {
            List* tokens = parser->_tokens->_first;
            Any* appObj;
            if (tokens->isEmpty()) {
                appObj = new Nil();
            }
            else {
                Any* abstrObj = tokens->_first;
                Any* restObj = tokens->_rest;
                if (restObj->_typeId != T_List) {
                    Symbol* exceptionSymbol = new Symbol("ProperListExpected");
                    Any** elems = new Any*[2]{exceptionSymbol,
                                              new Integer(parser->_pos)};
                    Array* exnAry = new Array(2, elems);
                    etor->_exception = exnAry;
                    etor->_status = ES_Exception;
                    return;
                }
                List* args = (List*)restObj;
                appObj = new Apply(abstrObj, args);
            }
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(appObj);
            etor->pushExpr(contin);
        }
    }

    static void _makeArray(Evaluator* etor, Parser* parser, Continuation* contin) {
        if (_checkClose(etor, parser, '}')) {
            Array* ary = Array::fromQueue(parser->_tokens);
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(ary);
            etor->pushExpr(contin);
        }
    }

    static void _makeList(Evaluator* etor, Parser* parser, Continuation* contin) {
        if (_checkClose(etor, parser, ']')) {
            List* list = parser->_tokens->_first;
            parser->_tokens = (Queue*)parser->_queueStack->_first;
            parser->_queueStack = (List*)parser->_queueStack->_rest;
            parser->_tokens->enq(list);
            etor->pushExpr(contin);
        }
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
        if (c == '"') {
            contin->_continFun = _parse;
            String* s = new String(parser->_lexeme.str());
            parser->_expectedClose = parser->_expectedClose->_next;
            _addToken(parser, s);
        }
        else if (c == '\0') {
            contin->_continFun = _parse;
        }
        else {
            parser->_lexeme << c;
        }
        etor->pushExpr(contin);
    }

    static void _parseSymbol(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (isspace(c) || iscntrl(c) || _isSpecialChar(c)) {
            parser->_pos--;
            contin->_continFun = _parse;
            Any* token = _checkSymbol(parser->_lexeme.str());
            _addToken(parser, token);
        }
        else {
            parser->_lexeme << c;
        }
        etor->pushExpr(contin);
    }

    static void _parse(Evaluator* etor, Any* arg, Continuation* contin) {
        Parser* parser = (Parser*)arg;
        char c = parser->_inputString[parser->_pos++];
        if (isdigit(c)) {
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
            parser->_expectedClose = new Triple(new Integer('"'),
                                                new Integer(parser->_pos),
                                                parser->_expectedClose);
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
            if (!parser->_expectedClose->isEmpty()) {
                _parseException("Missing terminator", parser, etor);
            }
            else {
                // EOI
                Any* token = parser->_tokens->_first;
                etor->pushObj(token);
            }
        }
        else {
            parser->_lexeme << c;
            contin->_name = "parser:symbol";  // TODO after this all works, don't bother changing the name of the parser anywhere
            contin->_continFun = _parseSymbol;
            etor->pushExpr(contin);
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
