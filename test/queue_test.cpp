#include <iostream>

#include <catch2/catch.hpp>

#include "src/integer.h"
#include "src/queue.h"

namespace plx {

    TEST_CASE("queue", "[queue]") {

        Queue* q = new Queue();

        SECTION("create") {
            REQUIRE(q != nullptr);
        }

        SECTION("enq deq") {
            REQUIRE(0 == q->_count);
            Integer* i100 = new Integer(100);
            Integer* i200 = new Integer(200);
            Integer* i300 = new Integer(300);
            q->enq(i100);
            q->enq(i200);
            q->enq(i300);
            REQUIRE(3 == q->_count);
            REQUIRE(i100 == q->deq_unsafe());
            REQUIRE(i200 == q->deq_unsafe());
            REQUIRE(i300 == q->deq_unsafe());
            REQUIRE(0 == q->_count);
        }

        SECTION("deq empty") {
            REQUIRE(nullptr == q->deq_unsafe());
        }

        THE_MEMORY.freeAll();
    }

}
