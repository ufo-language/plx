#include "src/array.h"

namespace plx {

    Array::Array(int count, Any** elems)
        : Any{T_Array}, _count{count}, _elems{elems} {
    }

    void Array::show(std::ostream& stream) {
        stream << '{';
        for (int n=0; n<_count; n++) {
            if (n > 0) {
                stream << ", ";
            }
            _elems[n]->show(stream);
        }
        stream << '}';
    }

}
