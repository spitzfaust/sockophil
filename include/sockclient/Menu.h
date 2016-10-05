//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>

#include "sockclient/ClientSelection.h"

namespace sockclient {
    /**
     * @class Menu Menu.h "sockclient/Menu.h"
     * @brief Class that is used by the Client class to display the TUI.
     */
    class Menu {
        /**
         * @var  connected_on Contains a string that describes the connection. (e.g. 127.0.0.1:1337)
         * */
        std::string connected_on;
        /**
         * @var current_directory Holds the path of the current directory (path the program was started from).
         */
        std::string current_directory;
        /**
         * @var first_run Is set true in ctor and set false after welcome message has been printed to the screen.
         */
        bool first_run;
        /**
         * @var terminal_width Width of the terminal.
         */
        unsigned int terminal_width;
        /**
         * @var terminal_height Height of the terminal

         */
        unsigned int terminal_height;
        void render_hr(const char filling, const int bg_color, const int color) const noexcept;
        void render_centered_msg(const std::string& message, const char filling, const int bg_color, const int color) const noexcept;
        void render_welcome_msg() const noexcept;
        void render_main_menu() const noexcept;
        void update_terminal_dimensions() noexcept;
        std::string clear_eol() const noexcept;
    public:
        Menu(int port, std::string ip_address, std::string current_directory);
        ~Menu();
        ClientSelection get_option();
        std::string get_connected_on() const noexcept;
        std::string get_current_directory() const noexcept;
    };
}
