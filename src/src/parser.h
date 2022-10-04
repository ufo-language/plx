#include <sstream>

#include "any.h"

namespace plx {

    struct Evaluator;
    struct List;
    struct Queue;
    struct String;

    struct Parser : public Any {
        Parser();
        Queue* _tokens;
        List* _queueStack;
        Triple* _expectedClose;
        std::string _inputString;
        std::stringstream _lexeme;
        int _pos = 0;
        int _iValue = 0;
        int _rFrac = 0;
        int _rDivisor = 1;
        void parse(const std::string& inputString, Evaluator* etor);
        void parse(String* inputString, Evaluator* etor);
        void parse(Evaluator* etor);
        void show(std::ostream& stream) override;
    };

}
