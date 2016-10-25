//
// Created by tobias on 10/10/16.
//

#pragma once

#include <string>

namespace sockophil {
    /**
     * @class Package Package.h "sockophil/Package.h"
     * @brief This is the parent package interface for all the package types.
     */
    class Package {
    public:
        virtual ~Package() = 0;
        virtual std::string get_type() const noexcept  = 0;
    };
}

