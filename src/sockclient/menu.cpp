//
// Created by tobias on 01/10/16.
//
#include <string>
#include "sockclient/menu.h"

namespace sockclient {
    Menu::Menu(int port, std::string ip_address, std::string current_directory) : current_directory(current_directory), first_run(true) {
        this->listening_on = ip_address + ":" + std::to_string(port);
    }

    std::string Menu::get_current_directory() const noexcept {
        return this->current_directory;
    }

    std::string Menu::get_listening_on() const noexcept {
        return this->listening_on;
    }
}