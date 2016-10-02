//
// Created by tobias on 01/10/16.
//
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <rlutil/rlutil.h>
#include "sockclient/menu.h"

namespace sockclient {
    Menu::Menu(int port, std::string ip_address, std::string current_directory) : current_directory(current_directory), first_run(true) {
        this->listening_on = ip_address + ":" + std::to_string(port);
        this->update_terminal_dimensions();
    }

    Menu::~Menu() {

    }

    void Menu::render_welcome_msg() const noexcept {
        rlutil::saveDefaultColor();
        rlutil::setBackgroundColor(rlutil::GREEN);
        for (int i = 0; i < this->terminal_width; ++i) {
            std::cout << "-";
        }
        rlutil::setColor(rlutil::BLACK);
        std::cout << std::endl << "Welcome to the sockclient! :)" << std::endl;
        rlutil::setColor(rlutil::WHITE);
        for (int i = 0; i < this->terminal_width; ++i) {
            std::cout << "-";
        }
        std::cout << std::endl;
        rlutil::resetColor();
    }

    void Menu::update_terminal_dimensions() noexcept {
        struct winsize w_size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_size);
        this->terminal_width = w_size.ws_col;
        this->terminal_height = w_size.ws_row;
    }

    std::string Menu::get_current_directory() const noexcept {
        return this->current_directory;
    }

    std::string Menu::get_listening_on() const noexcept {
        return this->listening_on;
    }

    ClientSelection Menu::get_option() {
        this->update_terminal_dimensions();
        if(this->first_run) {
            this->render_welcome_msg();
            this->first_run = false;
        }
        return ClientSelection(sockophil::list, "");
    }
}