//
// Created by tobias on 02/10/16.
//
#include <string>
#include <unistd.h>
#include "sockclient/menu.h"
#include "gtest/gtest.h"

class MenuTest : public ::testing::Test {
protected:
    int port;
    std::string ip_address;
    std::string listening_on;
    std::string current_directory;
    sockclient::Menu * menu;
public:
    MenuTest() {
        this->port = 1337;
        this->ip_address = "127.0.0.1";
        this->listening_on = "127.0.0.1:1337";
        char current_directory[1024];
        getcwd(current_directory, sizeof(current_directory));
        this->current_directory = current_directory;
        this->menu = new sockclient::Menu(this->port,this->ip_address,this->current_directory);
    }
    ~MenuTest() {
        delete(this->menu);
    }

};

TEST_F(MenuTest, test_get_listening_on) {
    EXPECT_STREQ(menu->get_listening_on().c_str(), listening_on.c_str());
    EXPECT_STRNE(menu->get_listening_on().c_str(), ip_address.c_str());
    EXPECT_STRNE(menu->get_listening_on().c_str(), "");
}

TEST_F(MenuTest, test_get_current_directory) {
    EXPECT_STREQ(menu->get_current_directory().c_str(), current_directory.c_str());
    EXPECT_STRNE(menu->get_current_directory().c_str(), "");
}