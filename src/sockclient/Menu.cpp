//
// Created by tobias on 01/10/16.
//
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <rlutil/rlutil.h>
#include "sockclient/Menu.h"

namespace sockclient {

    /**
     * Constructor sets the member vars.
     * @param port the client listens to
     * @param ip_address the client listens to
     * @param current_directory the client is running in and downloads files to
     */
    Menu::Menu(int port, std::string ip_address, std::string current_directory)
            : current_directory(current_directory), first_run(true) {
        this->connected_on = ip_address + ":" + std::to_string(port);
        this->update_terminal_dimensions();
    }

    /**
     * Destructor
     * @todo check if necesary
     */
    Menu::~Menu() {

    }

    /**
     * Print a message that is centered horizontaly in the terminal
     * @param message is printed to the terminal
     * @param filling left and right from the message
     * @param bg_color is the background color (rlutil)
     * @param color is the font color (rlutil)
     */
    void Menu::render_centered_msg(const std::string& message, const char filling = ' ', const int bg_color = rlutil::BLACK, const int color = rlutil::LIGHTGREEN) const noexcept {
        rlutil::setBackgroundColor(bg_color);
        rlutil::setColor(color);
        int first_width = (message.length() % 2 ? this->terminal_width - 1 : this->terminal_width) / 2;
        for (int j = 0; j < (first_width - (message.length() / 2)); ++j) {
            std::cout << " ";
        }
        std::cout << message;
        for (int j = 0; j < ((this->terminal_width / 2) - (message.length() / 2)); ++j) {
            std::cout << " ";
        }
        rlutil::resetColor();
        std::cout << std::endl;
    }

    /**
     * Print a horizontal line to the terminal
     * @param filling character that is printed
     * @param bg_color is the background color (rlutil)
     * @param color is the font color (rlutil)
     */
    void Menu::render_hr(const char filling = ' ', const int bg_color = rlutil::BLACK, const int color = rlutil::LIGHTGREEN) const noexcept {
        rlutil::setBackgroundColor(rlutil::BLACK);
        rlutil::setColor(rlutil::LIGHTGREEN);
        for (int i = 0; i < this->terminal_width; ++i) {
            std::cout << filling;
        }
        std::cout << std::endl;
        rlutil::resetColor();
    }

    /**
     * Print a welcome message to the screen. Is only called once on start of the client.
     */
    void Menu::render_welcome_msg() const noexcept {
        std::string welcome_msg =  "sockclient v0.1";
        rlutil::saveDefaultColor();
        this->render_hr('x');
        this->render_centered_msg(welcome_msg);
        this->render_hr('x');
        rlutil::setBackgroundColor(rlutil::BLACK);
        rlutil::setColor(rlutil::LIGHTGREEN);
        std::cout << this->clear_eol() << std::endl;
        std::cout << "\u2714 Connected on:   " << this->get_connected_on() << this->clear_eol() << std::endl;
        std::cout << "\u2714 Downloading to: " << this->get_current_directory() << this->clear_eol()<< std::endl;
        rlutil::resetColor();
    }

    /**
     * Get the width and height of the terminal window.
     */
    void Menu::update_terminal_dimensions() noexcept {
        struct winsize w_size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_size);
        this->terminal_width = w_size.ws_col;
        this->terminal_height = w_size.ws_row;
    }

    /**
     * Get the current_directory
     * @return current_directory
     */
    std::string Menu::get_current_directory() const noexcept {
        return this->current_directory;
    }

    /**
     * Get the connected_on string
     * @return connected_on
     */
    std::string Menu::get_connected_on() const noexcept {
        return this->connected_on;
    }

    /**
     * Entry point for the interaction with the TUI
     *
     * Display a Menu for the User to choose from the different Client actions
     * @return the selection the user made
     */
    ClientSelection Menu::get_option() {
        this->update_terminal_dimensions();
        if(this->first_run) {
            this->render_welcome_msg();
            this->first_run = false;
        }
        return ClientSelection(sockophil::list, "");
    }

    /**
     * Return a string that clears until the end of line.
     *
     * Used for lines that should have background color.
     * @return string to clear to the end of line
     */
    std::string Menu::clear_eol() const noexcept {
        return "\x1B[K";
    }
}