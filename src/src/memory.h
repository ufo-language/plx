#pragma once

namespace plx {

    struct Any;

    struct Memory {
        Memory() : _spine{nullptr} {}
        ~Memory() { freeAll(); }
        Any* _spine;
        void freeAll();
        void registerObject(Any* obj);
    };

    extern Memory THE_MEMORY;

}
