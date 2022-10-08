#include "array.h"
#include "continuation.h"
#include "evaluator.h"
#include "integer.h"
#include "queue.h"

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

    static void _makeArray(Evaluator* etor, Any* arg, Continuation* contin) {
        (void)contin;
        Integer* nElemsInt = (Integer*)arg;
        int nElems = nElemsInt->_value;
        Array* ary = new Array(nElems);
        for (int n=0; n<nElems; n++) {
            ary->_elems[n] = etor->popObj();
        }
        etor->pushObj(ary);
    }

    void Array::evaluate(Evaluator* etor) {
        Continuation* contin = new Continuation("array", _makeArray, new Integer(_count));
        etor->pushExpr(contin);
        for (int n=0; n<_count; n++) {
            etor->pushExpr(_elems[n]);
        }
    }

    Triple* Array::match(Any* other, Triple* env) {
        Array* otherArray = (Array*)other;
        if (_count != otherArray->_count) {
            return nullptr;
        }
        for (int n=0; n<_count; n++) {
            env = Any::Match(_elems[n], otherArray->_elems[n], env);
            if (env == nullptr) {
                break;
            }
        }
        return env;
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
