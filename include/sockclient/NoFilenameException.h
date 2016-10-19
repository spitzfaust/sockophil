//
// Created by tobias on 05/10/16.
//

#pragma once

#include <exception>


namespace sockclient {
    class NoFilenameException : public std::exception {
    public:
        NoFilenameException();
        virtual const char* what() const noexcept;
    };
}
