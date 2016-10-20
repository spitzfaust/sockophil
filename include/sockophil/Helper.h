//
// Created by tobias on 12/10/16.
//

#pragma once

#include <string>

namespace sockophil {
    class Helper {
    public:
        static std::string parse_filename(const std::string &filepath);

        static unsigned int number_digits(long number);
    };
}