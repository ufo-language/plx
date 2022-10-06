#include <iostream>

#include <catch2/catch.hpp>

#include "src/array.h"
#include "src/evaluator.h"
#include "src/integer.h"
#include "src/list.h"
#include "src/parser.h"
#include "src/string.h"
#include "src/symbol.h"

namespace plx {

    TEST_CASE("parser", "[parser]") {
        std::cerr << "parser_test starting\n";

        Evaluator* etor = new Evaluator();
        Parser* parser = new Parser();

        SECTION("parse integer", "[integer]") {
            int n = 123;
            String* inputString = new String(std::to_string(n));
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Integer);
            Integer* i = (Integer*)firstToken;
            REQUIRE(i->_value == n);
        }

        SECTION("parse string", "[string]") {
            std::string s = "abc def ghi";
            String* inputString = new String('"' + s + '"');
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
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

        SECTION("parse identifier", "[identifier]") {
            std::string s = "xyz";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Identifier);
            Symbol* sym = (Symbol*)firstToken;
            REQUIRE(sym->_name == s);
        }

        SECTION("parse empty list", "[list]") {
            std::string s = "[]";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_List);
            List* list = (List*)firstToken;
            REQUIRE(list->isEmpty());
        }

        SECTION("parse nonempty list", "[list]") {
            std::string s = "[100 200]";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_List);
            List* list = (List*)firstToken;
            REQUIRE(!list->isEmpty());
            Any* elem = list->_first;
            REQUIRE(elem->_typeId == T_Integer);
            Integer* i = (Integer*)elem;
            REQUIRE(i->_value == 100);
            list = (List*)list->_rest;
            REQUIRE(!list->isEmpty());
            elem = list->_first;
            REQUIRE(elem->_typeId == T_Integer);
            i = (Integer*)elem;
            REQUIRE(i->_value == 200);
            list = (List*)list->_rest;
            REQUIRE(list->isEmpty());
        }

        SECTION("parse empty array", "[array]") {
            std::string s = "{}";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Array);
            Array* ary = (Array*)firstToken;
            REQUIRE(ary->_count == 0);
        }

        SECTION("parse nonempty array", "[array]") {
            std::string s = "{100 200}";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Array);
            Array* ary = (Array*)firstToken;
            REQUIRE(ary->_count == 2);
            Any* elem = ary->_elems[0];
            REQUIRE(elem->_typeId == T_Integer);
            Integer* i = (Integer*)elem;
            REQUIRE(i->_value == 100);
            elem = ary->_elems[1];
            REQUIRE(elem->_typeId == T_Integer);
            i = (Integer*)elem;
            REQUIRE(i->_value == 200);
        }

        SECTION("parse apply", "[apply]") {
            std::string s = "(abc 123)";
            String* inputString = new String(s);
            List* args = new List(inputString);
            etor->pushObj(args);
            parser->parse(etor);
            etor->run();
            Any* res = etor->popObj();
            REQUIRE(res->_typeId == T_List);
            List* tokens = (List*)res;
            REQUIRE(!tokens->isEmpty());
            REQUIRE(tokens->_rest->_typeId == T_List);
            REQUIRE(((List*)tokens->_rest)->isEmpty());
            Any* firstToken = tokens->_first;
            REQUIRE(firstToken->_typeId == T_Apply);
        }

        THE_MEMORY.freeAll();
        std::cerr << "parser_test ending\n";
    }

}
