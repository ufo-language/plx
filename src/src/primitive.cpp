#include "src/primitive.h"

namespace plx {

    void Primitive::show(std::ostream& stream) {
        stream << _name << '@' << (void*)_primFun;
    }

}
