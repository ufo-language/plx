#include "src/array.h"
#include "src/queue.h"

namespace plx {

    Array* Array::fromQueue(Queue* q) {
        int count = q->_count;
        Array* ary = new Array(count);
        for (int n=0; n<count; n++) {
            ary->_elems[n] = q->deq_unsafe();
        }
        return ary;
    }

    Array::Array(int count)
        : Any{T_Array}, _count{count} {
        _elems = new Any*[count];
    }

    Array::Array(int count, Any** elems)
        : Any{T_Array}, _count{count}, _elems{elems} {
    }

    void Array::show(std::ostream& stream) {
        stream << '{';
        for (int n=0; n<_count; n++) {
            if (n > 0) {
                stream << ' ';
            }
            _elems[n]->show(stream);
        }
        stream << '}';
    }

}
