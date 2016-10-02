//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>

#include "sockclient/client_selection.h"

namespace sockclient {
    class Menu {
        std::string listening_on;
        std::string current_directory;
        bool first_run;
        void render_welcome_msg() const noexcept;
        void render_main_menu() const noexcept;
    public:
        Menu(int port, std::string ip_address, std::string current_directory);
        ClientSelection get_option();
        std::string get_listening_on() const noexcept;
        std::string get_current_directory() const noexcept;
    };
}
