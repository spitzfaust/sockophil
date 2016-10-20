//
// Created by tobias on 20/10/16.
//
#include <string>
#include "sockophil/Helper.h"
#include "gtest/gtest.h"


TEST(HelperTest, test_parse_filename) {
    EXPECT_STREQ("test.txt", sockophil::Helper::parse_filename("/home/user/Downloads/test.txt").c_str());
    EXPECT_STREQ("test.txt", sockophil::Helper::parse_filename("test.txt").c_str());
    EXPECT_STREQ("test.txt", sockophil::Helper::parse_filename("./test.txt").c_str());
    EXPECT_STREQ("test.txt", sockophil::Helper::parse_filename("~/Desktop/test.txt").c_str());
    EXPECT_STREQ("testfile", sockophil::Helper::parse_filename("home/user/testfile").c_str());
}

TEST(HelperTest, test_add_trailing_slash) {
    EXPECT_STREQ("./uploads/", sockophil::Helper::add_trailing_slash("./uploads").c_str());
    EXPECT_STREQ("./uploads/", sockophil::Helper::add_trailing_slash("./uploads").c_str());
    EXPECT_STREQ("/home/user/", sockophil::Helper::add_trailing_slash("/home/user").c_str());
    EXPECT_STREQ("./", sockophil::Helper::add_trailing_slash(".").c_str());
}