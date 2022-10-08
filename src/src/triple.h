#pragma once

#include "any.h"
#include "symbol.h"

namespace plx {

    struct Triple : public Any {
        Triple() : Any{T_Triple}, _key{nullptr}, _value{nullptr}, _next{nullptr} {}
        Triple(Any* key, Any* value, Triple* next) : Any{T_Triple}, _key{key}, _value{value}, _next{next} {}
        Any* _key;
        Any* _value;
        Triple* _next;
        bool boolValue() override { return !isEmpty(); }
        bool isEmpty() { return _key == nullptr; }
        Triple* locate(Any* name);
        void show(std::ostream& stream);
    };
    
}
