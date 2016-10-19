//
// Created by tobias on 12/10/16.
//

#include "sockophil/Helper.h"

std::string Helper::parse_filename(const std::string &filepath) {
    auto const pos = filepath.find_last_of('/');
    if(pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos+1);
}