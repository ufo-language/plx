#include "src/any.h"
#include "src/continuation.h"
#include "src/evaluator.h"
#include "src/triple.h"

namespace plx {

    std::ostream& operator << (std::ostream& stream, std::nullptr_t) {
        return stream << "nullptr";
    }

    std::ostream& operator << (std::ostream &stream, Any* object) {
        if(object == nullptr) {
            stream << "NULL-OBJECT";
        }
        else {
            object->show(stream);
        }
        return stream;
    }

    bool Any::IsEqual(Any* self, Any* other) {
        if (self == other) {
            return true;
        }
        if (self->_typeId != other->_typeId) {
            return false;
        }
        return self->isEqual(other);
    }

    void Any::evaluate(Evaluator* etor) {
        etor->pushObj(this);
    }

    Triple* Any::match(Any* other, Triple* env) {
        return Any::IsEqual(this, other) ? env : nullptr;
    }

}
