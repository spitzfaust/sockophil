//
// Created by tobias on 05/10/16.
//
#include "gtest/gtest.h"
#include "sockclient/NoFilenameException.h"
#include "sockophil/protocol.h"
#include "sockclient/ClientSelection.h"

TEST(ClientSelectionTest, test_get_action) {
    auto cs1 = sockclient::ClientSelection(sockophil::list);
    auto cs2 = sockclient::ClientSelection(sockophil::put, "hello");
    EXPECT_EQ(sockophil::list, cs1.get_action());
    EXPECT_EQ(sockophil::put, cs2.get_action());
}

TEST(ClientSelectionTest, test_get_filename) {
    auto cs1 = sockclient::ClientSelection(sockophil::list);
    auto cs2 = sockclient::ClientSelection(sockophil::put, "hello");
    EXPECT_STREQ("", cs1.get_filename().c_str());
    EXPECT_STREQ("hello", cs2.get_filename().c_str());
}

TEST(ClientSelectionTest, test_exception_throwen_if_no_filename) {
    EXPECT_THROW(sockclient::ClientSelection(sockophil::put), sockclient::NoFilenameException);
    EXPECT_THROW(sockclient::ClientSelection(sockophil::get, ""), sockclient::NoFilenameException);

}
