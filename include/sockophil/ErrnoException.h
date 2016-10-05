//
// Created by tobias on 03/10/16.
//

#pragma once

#include <exception>
#include <string>

namespace sockophil {
    /**
     * @class ErrnoException ErrnoException.h "sockophil/ErrnoException.h"
     * @brief Class that inherits from std::exception and is used as parent for C errors with errno set
     */
    class ErrnoException : public std::exception {
    protected:
        /**
         * @var error_number is the errno of the Error
         */
        int error_number;
        /**
         * @var error_test is the text returned for the errno by std::strerror()
         */
        std::string error_text;
    public:
        ErrnoException(int error_number);
        virtual const char* what() const noexcept;
    };
}

