#pragma once

#include <iostream>
#include <string>

#include "src/memory.h"

namespace plx {

    enum TypeId {
        T_NULL = 0,
        T_Apply,
        T_Array,
        T_Boolean,
        T_Continuation,
        T_Evaluator,
        T_ExceptionHandler,
        T_Function,
        T_Identifier,
        T_Integer,
        T_List,
        T_Nil,
        T_Parser,
        T_Prim,
        T_PrimMacro,
        T_Queue,
        T_Real,
        T_String,
        T_Symbol,
        T_Triple,
        T_FINAL
    };

    enum EvaluatorStatus {
        ES_Running,
        ES_Blocked,
        ES_Exception,
        ES_Terminated
    };

    std::ostream& operator << (std::ostream& stream, std::nullptr_t ptr);

    struct Evaluator;
    struct Triple;

    struct Any {
        Any(TypeId typeId) : _typeId{typeId} { THE_MEMORY.registerObject(this); }
        virtual ~Any() {};
        enum TypeId _typeId;
        Any* _next;
        bool _isMarked;
        virtual bool boolValue() { return true; }
        virtual void display(std::ostream& stream) { show(stream); }
        virtual void evaluate(Evaluator* etor);
        static bool IsEqual(Any* self, Any* other);
        virtual bool isEqual(Any* other) { (void)other; return false; }
        virtual Triple* match(Any* other, Triple* env);
        virtual void show(std::ostream& stream) = 0;
        friend std::ostream & operator<<(std::ostream &stream, Any* object);
    };

}
