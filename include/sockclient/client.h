//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>
#include <memory>
#include "sockclient/menu.h"

namespace sockclient {

    class Client {
        int port;
        std::string ip_address;
        std::string current_directory;
        std::shared_ptr<Menu> menu;
    public:
        Client(int port, std::string ip_address);
        ~Client();
        void run();
    };

}