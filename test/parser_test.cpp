#include <iostream>

#include <catch2/catch.hpp>

#include "src/evaluator.h"
#include "src/integer.h"
#include "src/list.h"
#include "src/parser2.h"
#include "src/string.h"
#include "src/symbol.h"

namespace plx {

    void runEvaluator(Evaluator* etor) {
        while (etor->_status == ES_Running) {
            etor->step();
        }
    }

    TEST_CASE("parser", "[parser]") {
        std::cerr << "parser_test starting\n";

        Evaluator* etor = new Evaluator();
        Parser* parser = new Parser();

        SECTION("create") {
            REQUIRE(parser != nullptr);
        }

        SECTION("parse integer", "[integer]") {
            int n = 123;
            String* str123 = new String(std::to_string(n));
            List* args = new List(str123);
            etor->pushObj(args);
            parser->parse(etor);
            runEvaluator(etor);
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Integer);
            Integer* i = (Integer*)firstToken;
            REQUIRE(i->_value == 123);
        }

        SECTION("parse string", "[string]") {
            std::string s = "abc def ghi";
            String* symAbc = new String('"' + s + '"');
            List* args = new List(symAbc);
            etor->pushObj(args);
            parser->parse(etor);
            runEvaluator(etor);
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_String);
            String* str = (String*)firstToken;
            REQUIRE(str->_str == s);
        }

        SECTION("parse symbol", "[symbol]") {
            std::string s = "xyz";
            String* symAbc = new String(s);
            List* args = new List(symAbc);
            etor->pushObj(args);
            parser->parse(etor);
            runEvaluator(etor);
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Symbol);
            Symbol* sym = (Symbol*)firstToken;
            REQUIRE(sym->_name == s);
        }

        THE_MEMORY.freeAll();
        std::cerr << "parser_test ending\n";
    }

}
