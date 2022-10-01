#include "src/array.h"
#include "src/evaluator.h"
#include "src/list.h"
#include "src/queue.h"
#include "src/symbol.h"

namespace plx {

    void Queue::enq(Any* elem) {
        List* pair = new List(elem);
        if (_first->isEmpty()) {
            _first = _last = pair;
        }
        else {
            _last->_rest = pair;
            _last = pair;
        }
        _count++;
    }

    void Queue::deq(Evaluator* etor) {
        if (_count == 0) {
            Any* elems[] = {new Symbol("QueueEmpty")};
            etor->_exception = new Array(1, elems);
        }
        else {
            etor->pushObj(deq_unsafe());
        }
    }

    Any* Queue::deq_unsafe() {
        Any* elem = _first->_first;
        _first = (List*)_first->_rest;
        _count--;
        return elem;
    }

    void Queue::show(std::ostream& stream) {
        _first->showWith(stream, "~[", "]");
    }

}
