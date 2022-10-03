#include <sstream>

#include "any.h"

namespace plx {

    struct Evaluator;
    struct List;
    struct Queue;

    struct Parser : public Any {
        Parser();
        int _pos = 0;
        std::string _inputString;
        std::stringstream _lexeme;
        Queue* _tokens;
        void parse(Evaluator* etor);
        void show(std::ostream& stream) override;
    };

}
