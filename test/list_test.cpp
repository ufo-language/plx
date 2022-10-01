#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

namespace plx {

    TEST_CASE("list", "[list]") {
#if 0
        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
        GLOBALS.setup();

        SECTION("create") {
            REQUIRE(GLOBALS.emptyList());
            REQUIRE(GLOBALS.emptyList()->isEmpty());
            D_List* list1 = D_List::create(GLOBALS.nil(), GLOBALS.nil());
            REQUIRE(!list1->isEmpty());
        }

        SECTION("mark children", "[gc]") {
            D_Integer* i100 = D_Integer::create(100);
            D_Integer* i200 = D_Integer::create(200);
            THE_GC.commit();
            D_List* list1 = D_List::create(i100, i200);
            REQUIRE(!i100->isMarked());
            REQUIRE(!i200->isMarked());
            REQUIRE(!list1->isMarked());
            THE_GC.mark();
            REQUIRE(i100->isMarked());
            REQUIRE(i200->isMarked());
            REQUIRE(list1->isMarked());        
        }

        SECTION("empty list") {
            REQUIRE_THROWS(GLOBALS.emptyList()->getFirst());
            REQUIRE_THROWS(GLOBALS.emptyList()->getRest());
            REQUIRE_THROWS(GLOBALS.emptyList()->setFirst(GLOBALS.nil()));
            REQUIRE_THROWS(GLOBALS.emptyList()->setRest(GLOBALS.nil()));
            REQUIRE(GLOBALS.emptyList()->isEmpty());
        }

        SECTION("get and set") {
            D_Integer* i100 = D_Integer::create(100);
            D_Integer* i200 = D_Integer::create(200);
            D_List* list1 = D_List::create(i100, i200);
            REQUIRE(i100 == list1->getFirst());
            REQUIRE(i200 == list1->getRest());
            D_Integer* i300 = D_Integer::create(300);
            D_Integer* i400 = D_Integer::create(400);
            list1->setFirst(i300);
            list1->setRest(i400);
            REQUIRE(i300 == list1->getFirst());
            REQUIRE(i400 == list1->getRest());
        }

        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
#endif
    }

}
