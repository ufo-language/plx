#include "any.h"
#include "continuation.h"
#include "evaluator.h"
#include "symbol.h"
#include "triple.h"

namespace plx {

    std::ostream& operator << (std::ostream& stream, std::nullptr_t) {
        return stream << "nullptr";
    }

    std::ostream& operator << (std::ostream &stream, Any* object) {
        if(object == nullptr) {
            stream << "(Any*)nullptr";
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

    Triple* Any::Match(Any* self, Any* other, Triple* env) {
        if (self == other) {
            return env;
        }
        if (self->_typeId == T_Identifier) {
            return ((Symbol*)self)->match(other, env);
        }
        if (self->_typeId == other->_typeId) {
            return self->match(other, env);
        }
        return nullptr;
    }

    Triple* Any::match(Any* other, Triple* env) {
        return this->isEqual(other) ? env : nullptr;
    }

}
