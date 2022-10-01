#pragma once

#include "src/any.h"

namespace plx {

    struct String : public Any {
        String(const std::string& str) : Any{T_String}, _str{str} {}
        const std::string _str;
        void show(std::ostream& stream) override { stream << '"' << _str << '"'; }
    };

}
