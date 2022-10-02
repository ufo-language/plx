#include "src/triple.h"

namespace plx {

    Triple* Triple::locate(Any* name) {
        Triple* triple = this;
        while (!triple->isEmpty()) {
            if (Any::IsEqual(triple->_key, name)) {
                return triple;
            }
            triple = triple->_next;
        }
        return nullptr;
    }

    void Triple::show(std::ostream& stream) {
        Triple* triple = this;
        stream << '[';
        bool firstIter = true;
        while (!triple->isEmpty()) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                stream << ' ';
            }
            stream << triple->_key << '=' << triple->_value;
            triple = triple->_next;
        }
        stream << ']';
    }

}
