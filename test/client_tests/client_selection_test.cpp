//
// Created by tobias on 05/10/16.
//
#include "gtest/gtest.h"
#include "sockclient/NoFilenameException.h"
#include "sockophil/Protocol.h"
#include "sockclient/ClientSelection.h"

TEST(ClientSelectionTest, test_get_action) {
    auto cs1 = sockclient::ClientSelection(sockophil::LIST);
    auto cs2 = sockclient::ClientSelection(sockophil::PUT, "hello");
    EXPECT_EQ(sockophil::LIST, cs1.get_action());
    EXPECT_EQ(sockophil::PUT, cs2.get_action());
}

TEST(ClientSelectionTest, test_get_filename) {
    auto cs1 = sockclient::ClientSelection(sockophil::LIST);
    auto cs2 = sockclient::ClientSelection(sockophil::PUT, "hello");
    EXPECT_STREQ("", cs1.get_filename().c_str());
    EXPECT_STREQ("hello", cs2.get_filename().c_str());
}

TEST(ClientSelectionTest, test_exception_throwen_if_no_filename) {
    EXPECT_THROW(sockclient::ClientSelection(sockophil::PUT), sockclient::NoFilenameException);
    EXPECT_THROW(sockclient::ClientSelection(sockophil::GET, ""), sockclient::NoFilenameException);

}
