#include <sstream>

#include "src/any.h"
#include "src/apply.h"
#include "src/boolean.h"
#include "src/integer.h"
#include "src/list.h"
#include "src/nil.h"
#include "src/queue.h"
#include "src/real.h"
#include "src/string.h"
#include "src/symbol.h"

namespace plx {

    enum ParseState {
        PS_Init,
        PS_Integer,
        PS_Real,
        PS_String,
        PS_Symbol
    };

    Any* parseSymbol(const std::string& name) {
        if (name == "nil") {
            return new Nil();
        }
        if (name == "true") {
            return new Boolean(true);
        }
        if (name == "false") {
            return new Boolean(false);
        }
        return new Symbol(name);
    }

    static List* _parse(const std::string& inputString, int index);

    List* parse(const std::string& inputString) {
        return _parse(inputString, 0);
    }

    static List* _parse(const std::string& inputString, int index) {
        ParseState parseState = PS_Init;
        std::stringstream lexeme;
        bool contin = true;
        Queue* expr = new Queue();
        List* exprStack = new List();
        int iValue = 0;
        int rFrac = 0;
        int rDivisor = 1;
        bool parsingApplication = false;
        Any* token = nullptr;
        while (contin) {
            char c = inputString[index++];
            //std::cout << "_parse state = " << parseState << ", c = '" << c << "', lexeme = '" << lexeme.str() << "'\n";
            switch (parseState) {
                case PS_Init:
                    if (c == '\0') { contin = false; }
                    else if (std::isalpha(c)) { lexeme << c; parseState = PS_Symbol; }
                    else if (std::isblank(c)) {}
                    else if (std::isdigit(c)) { iValue = c - '0'; parseState = PS_Integer; }
                    else if (c == '"') { parseState = PS_String; }
                    else if (c == '[') {
                        exprStack = new List(expr, exprStack);
                        expr = new Queue();
                        lexeme = std::stringstream();
                    }
                    else if (c == ']') {
                        token = expr->_first;
                        expr = (Queue*)exprStack->_first;
                        exprStack = (List*)exprStack->_rest;
                        goto MAKE_TOKEN;
                    }
                    else if (c == Apply::START_CHAR) {
                        exprStack = new List(expr, exprStack);
                        expr = new Queue();
                        lexeme = std::stringstream();
                        parsingApplication = true;
                    }
                    else if (c == Apply::STOP_CHAR) {
                        if (!parsingApplication) {
                            std::cerr << "PS_Init ending token '" << Apply::STOP_CHAR
                                << "' with no start token '" << Apply::START_CHAR << "'\n";
                            return nullptr;
                        }
                        if (expr->_count == 0) {
                            std::cerr << "PS_Init empty application\n";
                            return nullptr;
                        }
                        Any* abstr = expr->_first->_first;
                        List* appArgs = (List*)expr->_first->_rest;
                        token = new Apply(abstr, appArgs);
                        expr = (Queue*)exprStack->_first;
                        exprStack = (List*)exprStack->_rest;
                        parsingApplication = false;
                        goto MAKE_TOKEN;
                    }
                    else {
                        std::cerr << "PS_Init unhandled character '" << c << "'\n";
                        return nullptr;
                    }
                    break;
                case PS_Integer:
                    if (std::isdigit(c)) {
                        iValue = iValue * 10 + (c - '0');
                    }
                    else if (c == '.') {
                        parseState = PS_Real;
                    }
                    else {
                        token = new Integer(iValue);
                        iValue = 0;
                        index--;
                        goto MAKE_TOKEN;
                    }
                    break;
                case PS_Real:
                    if (std::isdigit(c)) {
                        rFrac = rFrac * 10.0 + (c - '0');
                        rDivisor *= 10;
                    }
                    else {
                        double rValue = ((double)iValue) + ((double)rFrac / (double)(rDivisor));
                        Real* real = new Real(rValue);
                        expr->enq(real);
                        rValue = 0;
                        rFrac = 0;
                        index--;
                        goto MAKE_TOKEN;
                    }
                    break;
                case PS_String:
                    if (c == '\"') {
                        token = new String(lexeme.str());
                        goto MAKE_TOKEN;
                    }
                    else {
                        lexeme << c;
                    }
                    break;
                case PS_Symbol:
                    if (std::isalpha(c)) {
                        lexeme << c;
                    }
                    else {
                        index--;
                        token = parseSymbol(lexeme.str());
                        goto MAKE_TOKEN;
                    }
                    break;
            }
            continue;
        MAKE_TOKEN:
            expr->enq(token);
            lexeme.str(std::string());
            parseState = PS_Init;
        }  // end while loop
        if (parseState != PS_Init) {
            std::cerr << "Parse error\n";
        }
        return expr->_first;
    }

}
