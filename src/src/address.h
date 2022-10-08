#pragma once

#include "any.h"

namespace plx {

    struct Address : public Any {
        Address(void* address) : Any{T_Address}, _address{address} {}
        void* _address;
        void show(std::ostream& stream) override { stream << _address; }
    };

}
