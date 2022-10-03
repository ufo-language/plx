#include <iostream>

#include <catch2/catch.hpp>

#include "src/evaluator.h"
#include "src/list.h"
#include "src/parser2.h"
#include "src/string.h"

namespace plx {

    TEST_CASE("parser", "[parser]") {
        std::cerr << "parser_test starting\n";

        Evaluator* etor = new Evaluator();
        Parser* parser = new Parser();

        SECTION("create") {
            std::cout << "parser_test / create\n";
            REQUIRE(parser != nullptr);
        }

        SECTION("test1") {
            std::cout << "parser_test / test1\n";
            String* str123 = new String("123");
            List* args = new List(str123);
            etor->pushObj(args);
            parser->parse(etor);
            while (etor->_status == ES_Running) {
                etor->step();
                assert(etor->_status >= ES_Running && etor->_status <= ES_Terminated);
            }
            std::cout << "parser_test got here 1, status = " << etor->_status << "\n";
            Any* res = etor->popObj();
            std::cout << "parser_test res = " << res << "\n";
        }

        THE_MEMORY.freeAll();
        std::cerr << "parser_test ending\n";
    }

}
