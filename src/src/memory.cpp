#include "src/any.h"

namespace plx {

    Memory THE_MEMORY;

    void Memory::freeAll() {
        Any* objects = _spine;
        while (objects != nullptr) {
            Any* next = objects->_next;
            delete objects;
            objects = next;
        }
        _spine = nullptr;
    }

    void Memory::registerObject(struct Any* obj) {
        obj->_next = _spine;
        _spine = obj;
        obj->_isMarked = false;
    }

    // NB garbage collection can't be done during an evaluation step

}
