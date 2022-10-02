#include "src/apply.h"
#include "src/function.h"
#include "src/list.h"

namespace plx {

    void Function::show(std::ostream& stream) {
        stream << Apply::START_CHAR << "fun " << _params;
        _body->showWith(stream, " ", std::string(1, Apply::STOP_CHAR));
    }

}
