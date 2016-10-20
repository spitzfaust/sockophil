//
// Created by tobias on 12/10/16.
//

#include "sockophil/Helper.h"
namespace sockophil {
    std::string Helper::parse_filename(const std::string &filepath) {
        auto const pos = filepath.find_last_of('/');
        if (pos == std::string::npos) {
            return filepath;
        }
        return filepath.substr(pos + 1);
    }

    unsigned int Helper::number_digits(long number) {
        unsigned int digits = 0;
        if (number < 0) digits = 1;
        while (number) {
            number /= 10;
            digits++;
        }
        return digits;
    }
}