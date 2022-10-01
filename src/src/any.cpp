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

    bool Any::equalTo(Any* other) {
        if (this == other) {
            return true;
        }
        if (this->_typeId != other->_typeId) {
            return false;
        }
        return this->equalTo(other);
    }

    EvaluatorStatus Any::evaluate(Evaluator* etor) {
        etor->pushObj(this);
        return ES_Running;
    }

    Triple* Any::match(Any* other, Triple* env) {
        return equalTo(other) ? env : nullptr;
    }

}
