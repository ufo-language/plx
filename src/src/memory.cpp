#include "any.h"

namespace plx {

    Memory THE_MEMORY;

    void Memory::freeAll() {
        std::cout << "Memory::freeAll freeing " << _nRegisteredObjects << " objects\n";
        Any* objects = _spine;
        while (objects != nullptr) {
            Any* next = objects->_next;
            delete objects;
            objects = next;
        }
        _spine = nullptr;
        _nRegisteredObjects = 0;
    }

    void Memory::registerObject(struct Any* obj) {
        obj->_next = _spine;
        _spine = obj;
        obj->_isMarked = false;
        _nRegisteredObjects++;
    }

    // NB garbage collection can't be done during an evaluation step

}
