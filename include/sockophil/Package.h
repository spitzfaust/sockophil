//
// Created by tobias on 10/10/16.
//

#pragma once

#include <string>

namespace sockophil {
    class Package {
    public:
        virtual std::string get_type() const noexcept  = 0;
    };
}

